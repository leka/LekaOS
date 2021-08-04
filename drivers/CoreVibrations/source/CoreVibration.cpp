#include "CoreVibration.h"

using namespace leka;

using namespace std::chrono;

CoreVibration::CoreVibration(LKCoreSTM32HalBase &hal, interface::Dac &dac, interface::DacTimer &timer,
							 rtos::Thread &thread, events::EventQueue &eventQueue)
	: _hal(hal), _coreDac(dac), _coreTimer(timer), _thread(thread), _eventQueue(eventQueue)
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

void CoreVibration::deInit()
{
	_coreTimer.deInitialize();
	_coreDac.deInitialize();
}

void CoreVibration::play(VibrationTemplate &vib)
{
	stop();	  // stop current vib if playing and reset buffers

	_currentVib = &vib;	  // store current vib in order to access it from the callbacks

	_samplesPerPeriod = static_cast<uint32_t>(_samplingRate / static_cast<float>(vib.getFrequency()));
	// printf("Samples per period: %d\n", _samplesPerPeriod);

	_sinBuffer = new float[_samplesPerPeriod];
	_tmpBuffer = new float[_samplesPerPeriod];

	createSinWavePeriod(_sinBuffer, vib);

	uint32_t totalSamples = static_cast<uint16_t>(vib.getDuration().count() * _samplingRate);
	vib.setCurrentSample(0);
	vib.setTotalSamples(totalSamples);
	// printf("Samples remaining at start of vib: %d\n", _samplesRemaining);

	_vibBuffer_1 = new uint16_t[_samplesPerPeriod * 2];	  // buffer of 2 periods
	_vibBuffer_2 = _vibBuffer_1 + _samplesPerPeriod;

	// printf("Starting vib\n");
	start();
}

void CoreVibration::stop()
{
	// printf("Stoping vib\n");
	_coreTimer.stop();
	_coreDac.stop();

	delete[] _sinBuffer;
	_sinBuffer = nullptr;

	delete[] _tmpBuffer;
	_tmpBuffer = nullptr;

	delete[] _vibBuffer_1;
	_vibBuffer_1 = nullptr;

	_isPlaying	= false;
	_currentVib = nullptr;
}

// PRIVATE

void CoreVibration::start()
{
	fillHalfBuffer(_vibBuffer_1, _samplesPerPeriod);
	fillHalfBuffer(_vibBuffer_2, _samplesPerPeriod);
	_coreDac.start(_vibBuffer_1, _samplesPerPeriod * 2);
	_coreTimer.start();

	_isPlaying = true;
}

void CoreVibration::createSinWavePeriod(float *sinBuffer, VibrationTemplate &vib) const
{
	// offset and coef to have a positive sinwave of amplitude 1
	const uint16_t maxDacVal = 0xFFF;
	const double maxCoef	 = 0.4;
	double normalizeCoef	 = maxCoef * vib.getAmplitude() * maxDacVal;

	for (uint32_t i = 0; i < _samplesPerPeriod; ++i) {
		sinBuffer[i] = static_cast<float>(normalizeCoef * sin(i * 2 * M_PI / _samplesPerPeriod));
	}
}