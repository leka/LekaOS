#include "CoreEventQueue.h"

using namespace leka;

void CoreEventQueue::dispatch_forever()
{
	_event_queue_thread.start({&_event_queue, &events::EventQueue::dispatch_forever});
}
