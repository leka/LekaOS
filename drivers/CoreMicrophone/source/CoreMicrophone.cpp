#include "CoreMicrophone.h"

namespace leka {

float CoreMicrophone::readVolume(void)
{
	return _pin.read();
}

}	// namespace leka
