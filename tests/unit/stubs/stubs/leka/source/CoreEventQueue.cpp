#include "../CoreEventQueue.h"

namespace leka {

bool spy_CoreEventQueue_did_call_function = false;
std::function<void()> spy_CoreEventQueue_on_dispatch_forever_call;

void CoreEventQueue::dispatch_forever()
{
	if (spy_CoreEventQueue_on_dispatch_forever_call) {
		spy_CoreEventQueue_on_dispatch_forever_call();
	}
}

void CoreEventQueue::callMbedCallback(mbed::Callback<void()> const &f)
{
	f();
	spy_CoreEventQueue_did_call_function = true;
}

}	// namespace leka
