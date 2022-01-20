#include "CoreMicrophone.h"

#include "gtest/gtest.h"
#include "stubs/mbed/AnalogIn.h"

using namespace leka;

CoreMicrophone micro(PinName::MCU_MIC_INPUT);

float test_set_sound(float value)
{
	spy_AnalogIn_setValue(value);
	return value;
}

TEST(CoreMicrophoneTest, initialization)
{
	ASSERT_NE(&micro, nullptr);
}

TEST(CoreMicrophoneTest, readMinSound)
{
	auto expected = test_set_sound(0.0f);

	ASSERT_EQ(expected, micro.readVolume());
}

TEST(CoreMicrophoneTest, readAverageSound)
{
	auto expected = test_set_sound(0.50f);

	ASSERT_EQ(expected, micro.readVolume());
}

TEST(CoreMicrophoneTest, readMaxSound)
{
	auto expected = test_set_sound(1.0f);

	ASSERT_EQ(expected, micro.readVolume());
}
