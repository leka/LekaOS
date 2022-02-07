#include "../CoreEventQueue.h"

namespace leka {

bool spy_CoreEventQueue_did_call_function = false;

void CoreEventQueue::dispatch_forever()
{
	// do nothing
}

void CoreEventQueue::call(std::function<void()> const &f)
{
	f();
	spy_CoreEventQueue_did_call_function = true;
}

void CoreEventQueue::call(mbed::Callback<void()> const &f)
{
	f();
	spy_CoreEventQueue_did_call_function = true;
}

}	// namespace leka
