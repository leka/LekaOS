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