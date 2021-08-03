#include "CoreVibration.h"

using namespace leka;

using namespace std::chrono;

CoreVibration::CoreVibration(LKCoreSTM32HalBase &hal, interface::Dac &dac, interface::DacTimer &timer,
							 rtos::Thread &thread, events::EventQueue &eventQueue)
	: _hal(hal), _coreDac(dac), _coreTimer(timer), _thread(thread), _eventQueue(eventQueue)
{
	_thread.start({&_eventQueue, &events::EventQueue::dispatch_forever});
}