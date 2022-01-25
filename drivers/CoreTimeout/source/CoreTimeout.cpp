#include "CoreTimeout.h"

using namespace leka;
using namespace std::chrono;

void CoreTimeout::onTimeout(callback_t const &callback)
{
	_callback = callback;
}

void CoreTimeout::start(microseconds countdown)
{
	stop();
	_timeout.attach({this, &CoreTimeout::timeout}, countdown);
}

void CoreTimeout::stop()
{
	_timeout.detach();
}

void CoreTimeout::timeout() const
{
	_callback();
}
