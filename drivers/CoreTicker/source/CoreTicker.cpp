#include "CoreTicker.h"

using namespace leka;
using namespace std::chrono;

void CoreTicker::onTick(callback_t const &callback)
{
	_callback = callback;
}

void CoreTicker::start(microseconds delay)
{
	stop();
	_ticker.attach({this, &CoreTicker::tick}, delay);
}

void CoreTicker::stop()
{
	_ticker.detach();
}

void CoreTicker::tick() const
{
	_callback();
}
