#include "CoreEventQueue.h"

using namespace leka;

void CoreEventQueue::dispatch_forever()
{
	_event_queue_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
}

void CoreEventQueue::call(std::function<void()> const &f)
{
	_event_queue.call(f);
}

void CoreEventQueue::call(mbed::Callback<void()> const &f)
{
	_event_queue.call(f);
}
