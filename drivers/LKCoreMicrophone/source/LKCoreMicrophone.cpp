#include "LKCoreMicrophone.h"

namespace leka {

float LKCoreMicrophone::readRawValue(void)	 // TODO: make sure the method is useful and remove if needed
{
	return _pin.read();
}

float LKCoreMicrophone::readVolume(void)
{
	return readRawValue();
}

}	// namespace leka
