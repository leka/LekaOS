#include "LKCoreMicrophone.h"

namespace leka {

float LKCoreMicrophone::readVolume(void)
{
	return _pin.read();
}

}	// namespace leka
