#include "CoreVibration.h"

using namespace leka;

using namespace std::chrono;

CoreVibration::CoreVibration(LKCoreSTM32HalBase &hal, DACDriver &dac, DACTimer &timer, rtos::Thread &thread,
							 events::EventQueue &eventQueue)
	: _hal(hal), _coreDac(dac), _coreTimer(timer), _thread(thread), _eventQueue(eventQueue)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
}

void CoreVibration::initialize(uint32_t samplingRate)
{
	this->_samplingRate = samplingRate;

	// setup DAC callbacks
	static auto *self = this;
	auto halfBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->halfBufferCallback(); });
	auto fullBuffCb =
		static_cast<pDAC_CallbackTypeDef>([]([[maybe_unused]] DAC_HandleTypeDef *hdac) { self->cptBufferCallback(); });

	// initialize components
	_coreTimer.initialize(samplingRate);
	_coreDac.initialize(_coreTimer, halfBuffCb, fullBuffCb);
}

void CoreVibration::deInit()
{
	_coreTimer.terminate();
	_coreDac.terminate();
}

void CoreVibration::play(VibrationTemplate &vib)
{
	stop();	  // stop current vib if playing and reset buffers

	_currentVib = &vib;	  // store current vib in order to access it from the callbacks

	_samplesPerPeriod = _samplingRate / vib.getFrequency();

	_sinBuffer.resize(_samplesPerPeriod, 0.5);
	_tmpBuffer.resize(_samplesPerPeriod, 0.5);

	createSinWavePeriod(_sinBuffer.data(), vib);

	auto totalSamples = static_cast<uint32_t>(vib.getDuration().count() * static_cast<float>(_samplingRate));
	vib.setCurrentSample(0);
	vib.setTotalSamples(totalSamples);

	_vibBuffer.resize(_samplesPerPeriod * 2, 0x7FF);
	_vibBufferPtr_1 = _vibBuffer.data();
	_vibBufferPtr_2 = _vibBufferPtr_1 + _samplesPerPeriod;

	start();
}

void CoreVibration::playPeriodically(VibrationTemplate &vib, fseconds waitTime, uint16_t nbRep)
{
	if (nbRep > 0) {
		// getting duration in ms
		fseconds period = vib.getDuration() + waitTime;

		_eventQueue.call(this, &CoreVibration::playPtr, &vib);
		_isPlayingPeriodically = true;

		int id = 0;	  // id is null if no periodic call is made
		if (nbRep > 1) {
			// periodic call, will start in 1 period
			id = _eventQueue.call_every(std::chrono::duration_cast<milliseconds>(period), this, &CoreVibration::playPtr,
										&vib);
		}
		// end periodic call after the given time
		_eventQueue.call_in(std::chrono::duration_cast<milliseconds>(nbRep * period), this,
							&CoreVibration::endPeriodicVib, id);
	}
}

void CoreVibration::stop()
{
	_coreTimer.stop();
	_coreDac.stop();

	_isPlaying	= false;
	_currentVib = nullptr;
}

auto CoreVibration::isPlaying() const -> bool
{
	return _isPlaying;
}

auto CoreVibration::isPlayingPeriodically() const -> bool
{
	return _isPlayingPeriodically;
}

// PRIVATE

void CoreVibration::start()
{
	fillHalfBuffer(_vibBufferPtr_1, _samplesPerPeriod);
	fillHalfBuffer(_vibBufferPtr_2, _samplesPerPeriod);

	auto outSpan = lstd::span {_vibBufferPtr_1, _samplesPerPeriod * 2};	  // TODO() : check this
	_coreDac.start(outSpan);
	_coreTimer.start();

	_isPlaying = true;
}

void CoreVibration::createSinWavePeriod(float *sinBuffer, const VibrationTemplate &vib) const
{
	// offset and coef to have a positive sinwave of amplitude 1
	const uint16_t maxDacVal = 0xFFF;
	const double maxCoef	 = 0.4;
	double normalizeCoef	 = maxCoef * vib.getAmplitude() * maxDacVal;

	for (uint32_t i = 0; i < _samplesPerPeriod; ++i) {
		sinBuffer[i] = static_cast<float>(normalizeCoef * sin(i * 2 * M_PI / _samplesPerPeriod));
	}
}

void CoreVibration::halfBufferCallback()
{
	_eventQueue.call(this, &CoreVibration::handleCallback, _vibBufferPtr_1);
}

void CoreVibration::cptBufferCallback()
{
	_eventQueue.call(this, &CoreVibration::handleCallback, _vibBufferPtr_2);
}

void CoreVibration::handleCallback(u_int16_t *buffer)
{
	static uint8_t callsBeforeStop		= 2;
	static uint32_t lastPeriodThreshold = 0;
	if (lastPeriodThreshold == 0) {
		lastPeriodThreshold = _currentVib->getTotalSamples() - _samplesPerPeriod;
	}

	if (_currentVib->getCurrentSample() < lastPeriodThreshold) {
		fillHalfBuffer(buffer, _samplesPerPeriod);
		callsBeforeStop = 2;
	} else if (_currentVib->getCurrentSample() < _currentVib->getTotalSamples()) {
		uint32_t remaining = _currentVib->getTotalSamples() - _currentVib->getCurrentSample();
		fillHalfBuffer(buffer, remaining);
		callsBeforeStop = 1;
	} else if (callsBeforeStop == 1) {
		fillHalfBuffer(buffer, 0);
		callsBeforeStop = 0;
	} else if (callsBeforeStop == 0) {
		this->stop();
		callsBeforeStop		= 2;
		lastPeriodThreshold = 0;
	}
}

void CoreVibration::fillHalfBuffer(uint16_t *buffer, uint32_t nbSamples)
{
	const float normalizeOffset = 0.45 * 0xFFF;
	for (uint32_t i = 0; i < nbSamples; ++i) {
		_tmpBuffer[i] = _sinBuffer[i];
	}
	for (uint32_t i = nbSamples; i < _samplesPerPeriod; ++i) {
		buffer[i] = static_cast<u_int16_t>(normalizeOffset);
	}

	if (nbSamples > 0) {
		this->_currentVib->applyCurrentEnvelopeSlice(_tmpBuffer.data(), nbSamples);
		_currentVib->setCurrentSample(_currentVib->getCurrentSample() + nbSamples);
	}

	for (uint32_t i = 0; i < nbSamples; ++i) {
		_tmpBuffer[i] += normalizeOffset;

		buffer[i] = static_cast<u_int16_t>(_tmpBuffer[i]);
	}
}

void CoreVibration::endPeriodicVib(int eventID)
{
	_eventQueue.cancel(eventID);
	_isPlayingPeriodically = false;
}

void CoreVibration::playPtr(VibrationTemplate *vib)
{
	if (vib != nullptr) {
		play(*vib);
	}
}