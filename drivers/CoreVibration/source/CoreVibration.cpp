#include "CoreVibration.h"

using namespace leka;

using namespace std::chrono;

CoreVibration::CoreVibration(LKCoreSTM32HalBase &hal, interface::Dac &dac, interface::DacTimer &timer,
							 rtos::Thread &thread, events::EventQueue &eventQueue)
	: _hal(hal),
	  _coreDac(dac),
	  _coreTimer(timer),
	  _thread(thread),
	  _eventQueue(eventQueue),
	  _samplingRate(0),
	  _samplesPerPeriod(0),
	  _currentVib(nullptr)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
}

void CoreVibration::initialize(float samplingRate)
{
	this->_samplingRate = samplingRate;

	// setup DAC callbacks
	static auto *self = this;
	_coreDac.setCptCallbackPtr([](DAC_HandleTypeDef *hdac) { self->cptBufferCallback(); });
	_coreDac.setHalfCptCallbackPtr([](DAC_HandleTypeDef *hdac) { self->halfBufferCallback(); });

	// initialize components
	_coreTimer.initialize(samplingRate);
	_coreDac.initialize();
}

void CoreVibration::playPeriodically(VibrationTemplate &vib, fseconds waitTime, uint16_t nbRep)
{
	if (nbRep > 0) {
		// getting duration in ms
		fseconds period = vib.getDuration() + waitTime;
		// printf("duration : %f\nwaitTime : %f\nperiod : %f\n", vib.getDuration().count(), waitTime.count(),
		// period.count()); // first immediate call
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

void CoreVibration::endPeriodicVib(int eventID)
{
	_eventQueue.cancel(eventID);
	_isPlayingPeriodically = false;
}

void CoreVibration::playSequence(std::vector<VibrationTemplate> sequence)
{
	for (auto vib: sequence) {
		play(vib);
	}
}

void CoreVibration::play(VibrationTemplate &vib)
{
	stop();	  // stop current vib if playing and reset buffers

	_currentVib = &vib;	  // store current vib in order to access it from the callbacks

	_samplesPerPeriod = static_cast<uint32_t>(_samplingRate / vib.getFrequency());
	// printf("Samples per period: %d\n", _samplesPerPeriod);

	_sinBuffer = new uint16_t[_samplesPerPeriod];

	createSinWavePeriod(_sinBuffer, vib);

	uint32_t totalSamples = vib.getDuration().count() * _samplingRate;
	vib.setCurrentSample(0);
	vib.setTotalSamples(totalSamples);
	// printf("Samples remaining at start of vib: %d\n", _samplesRemaining);

	_vibBuffer_1 = new uint16_t[_samplesPerPeriod * 2];	  // buffer of 2 periods
	_vibBuffer_2 = _vibBuffer_1 + _samplesPerPeriod;

	// printf("Starting vib\n");
	start();
}

void CoreVibration::playPtr(VibrationTemplate *vib)
{
	if (vib != nullptr) {
		play(*vib);
	}
}

void CoreVibration::start()
{
	fillHalfBuffer(_vibBuffer_1, _samplesPerPeriod);
	fillHalfBuffer(_vibBuffer_2, _samplesPerPeriod);
	_coreDac.start(_vibBuffer_1, _samplesPerPeriod * 2);
	_coreTimer.start();

	_isPlaying = true;
}

void CoreVibration::stop()
{
	// printf("Stoping vib\n");
	_coreTimer.stop();
	_coreDac.stop();

	delete[] _sinBuffer;
	_sinBuffer = nullptr;
	delete[] _vibBuffer_1;
	_vibBuffer_1 = nullptr;

	_isPlaying	= false;
	_currentVib = nullptr;
}

void CoreVibration::deInit()
{
	_coreTimer.deInitialize();
	_coreDac.deInitialize();
}

void CoreVibration::createSinWavePeriod(uint16_t *sinBuffer, VibrationTemplate &vib)
{
	// printf("Sinbuffer: \n");
	// offset and coef to have a positive sinwave of amplitude 1
	double maxCoef		   = 0.4;
	double normalizeCoef   = maxCoef * vib.getAmplitude();
	double normalizeOffset = 0.45;

	for (uint32_t i = 0; i < _samplesPerPeriod; ++i) {
		double tmp = normalizeCoef * sin(i * 2.0 * M_PI / _samplesPerPeriod) + normalizeOffset;

		sinBuffer[i] = static_cast<uint16_t>(tmp * UINT16_MAX);
		// printf("%d\n", sinBuffer[i]);
	}
}

void CoreVibration::halfBufferCallback()
{
	_eventQueue.call(this, &CoreVibration::handleCallback, _vibBuffer_1);
}

void CoreVibration::cptBufferCallback()
{
	_eventQueue.call(this, &CoreVibration::handleCallback, _vibBuffer_2);
}

void CoreVibration::handleCallback(u_int16_t *buffer)
{
	static uint8_t callsBeforeStop			  = 2;
	static const uint32_t lastPeriodThreshold = _currentVib->getTotalSamples() - _samplesPerPeriod;

	if (_currentVib->getCurrentSample() < lastPeriodThreshold) {
		// printf("more than sPP\n");
		fillHalfBuffer(buffer, _samplesPerPeriod);
		callsBeforeStop = 2;
	} else if (_currentVib->getCurrentSample() < _currentVib->getTotalSamples()) {
		// printf("less than sPP\n");
		uint32_t remaining = _currentVib->getTotalSamples() - _currentVib->getCurrentSample();
		fillHalfBuffer(buffer, remaining);
		callsBeforeStop = 1;
	} else if (callsBeforeStop == 1) {
		// printf("last buff\n");
		fillHalfBuffer(buffer, 0);
		callsBeforeStop = 0;
	} else if (callsBeforeStop == 0) {
		// printf("end\n");
		this->stop();
		callsBeforeStop = 2;
	}

	// printf("rem: %d\ncall: %d\n", _samplesRemaining, callsBeforeStop);
}

void CoreVibration::fillHalfBuffer(uint16_t *buffer, uint32_t nbSamples)
{
	// printf("filling half buff\n");
	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] = _sinBuffer[i];
		// printf("\tb[%d] : %d\n", i, buffer[i]);
	}
	for (uint32_t i = nbSamples; i < _samplesPerPeriod; ++i) {
		buffer[i] = 0.45 * UINT16_MAX;
	}

	if (nbSamples > 0) {
		this->_currentVib->applyCurrentEnvelopeSlice(buffer, nbSamples);
		_currentVib->setCurrentSample(_currentVib->getCurrentSample() + nbSamples);
	}

	alignU16ToDac(buffer, nbSamples);
}

void CoreVibration::alignU16ToDac(uint16_t *buffer, uint32_t nbSamples)
{
	for (uint32_t i = 0; i < nbSamples; ++i) {
		buffer[i] = buffer[i] >> 4;
	}
}

auto CoreVibration::isPlaying() const -> bool
{
	return _isPlaying;
}

auto CoreVibration::isPlayingPeriodically() const -> bool
{
	return _isPlayingPeriodically;
}