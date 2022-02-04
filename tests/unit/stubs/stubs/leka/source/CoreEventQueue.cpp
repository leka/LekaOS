#include "../CoreEventQueue.h"

namespace leka {

bool spy_CoreEventQueue_did_call_function = false;

void CoreEventQueue::dispatch_forever()
{
	// do nothing
}

void CoreEventQueue::call(const std::function<void()> &f)
{
	f();
	spy_CoreEventQueue_did_call_function = true;
}

}	// namespace leka
