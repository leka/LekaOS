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
	  _samplesRemaining(0),
	  _samplingRate(0),
	  _samplesPerPeriod(0)
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

void CoreVibration::playVibrationSequence(std::vector<VibrationTemplate> sequence)
{
	for (auto vib: sequence) {
		playVibration(vib);
	}
}

void CoreVibration::playVibration(VibrationTemplate &vib)
{
	_samplesPerPeriod = static_cast<uint32_t>(_samplingRate / vib.getFrequency());

	_sinBuffer = new uint16_t[_samplesPerPeriod];

	createSinWavePeriod(_sinBuffer, vib);

	_samplesRemaining = vib.getDuration() * _samplingRate;

	_vibBuffer	 = new uint16_t[_samplesPerPeriod * 2];	  // buffer of 2 periods
	_vibBuffer_2 = _vibBuffer + _samplesPerPeriod;

	start();

	while (_samplesRemaining > 0) {
		rtos::ThisThread::sleep_for(50ms);
	}

	stopVibration();
}

void CoreVibration::start()
{
	fillHalfBuffer(_vibBuffer, _samplesPerPeriod);
	fillHalfBuffer(_vibBuffer_2, _samplesPerPeriod);
	_coreDac.start(_vibBuffer, _samplesPerPeriod * 2);
	_coreTimer.start();
}

void CoreVibration::stopVibration()
{
	_coreDac.stop();
	_coreTimer.stop();
}

void CoreVibration::deInit()
{
	_coreTimer.deInitialize();
	_coreDac.deInitialize();
	delete[] _sinBuffer;
	delete[] _vibBuffer;
}

void CoreVibration::createSinWavePeriod(uint16_t *sinBuffer, VibrationTemplate &vib)
{
	// offset and coef to have a positive sinwave of amplitude 1
	double maxCoef		   = 0.4;
	double normalizeCoef   = maxCoef * vib.getAmplitude();
	double normalizeOffset = 0.45;

	for (uint32_t i = 0; i < _samplesPerPeriod; ++i) {
		double tmp = normalizeCoef * sin(i * 2.0 * M_PI / _samplesPerPeriod) + normalizeOffset;

		sinBuffer[i] = static_cast<uint16_t>(tmp * UINT16_MAX);
	}
}

void CoreVibration::halfBufferCallback()
{
	_eventQueue.call(this, &CoreVibration::handleCallback, _vibBuffer);
}

void CoreVibration::cptBufferCallback()
{
	_eventQueue.call(this, &CoreVibration::handleCallback, _vibBuffer + _samplesPerPeriod);
}

void CoreVibration::handleCallback(u_int16_t *buffer)
{
	static uint8_t callsBeforeStop = 2;

	if (_samplesRemaining > _samplesPerPeriod) {
		fillHalfBuffer(buffer, _samplesPerPeriod);
		_samplesRemaining -= _samplesPerPeriod;
		callsBeforeStop = 2;
	} else if (_samplesRemaining > 0) {
		fillHalfBuffer(buffer, _samplesRemaining);
		_samplesRemaining -= _samplesPerPeriod;
		callsBeforeStop = 1;
	} else if (callsBeforeStop == 1) {
		fillHalfBuffer(buffer, 0);
		callsBeforeStop == 0;
	} else if (callsBeforeStop == 0) {
		this->stopVibration();
	}
}

void CoreVibration::fillHalfBuffer(uint16_t *buffer, uint32_t numOfSamples)
{
	for (uint32_t i = 0; i < numOfSamples; ++i) {
		buffer[i] = _sinBuffer[i] >> 4;
	}
	for (uint32_t i = numOfSamples; i < _samplesPerPeriod; ++i) {
		buffer[i] = 0;
	}
}