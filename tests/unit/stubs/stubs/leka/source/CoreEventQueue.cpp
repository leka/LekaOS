#include "../CoreEventQueue.h"

namespace leka {

std::function<void()> spy_CoreEventQueue_on_dispatch_forever_call;

void CoreEventQueue::dispatch_forever()
{
	if (spy_CoreEventQueue_on_dispatch_forever_call) {
		spy_CoreEventQueue_on_dispatch_forever_call();
	}
}

}	// namespace leka
