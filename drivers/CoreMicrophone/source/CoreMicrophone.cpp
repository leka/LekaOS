#include "CoreMicrophone.h"

namespace leka {

auto CoreMicrophone::readVolume() -> float
{
	return _pin.read();
}

}	// namespace leka
