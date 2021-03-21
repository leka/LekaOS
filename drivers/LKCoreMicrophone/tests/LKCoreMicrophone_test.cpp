#include "LKCoreMicrophone.h"

#include "gtest/gtest.h"
#include "stub_AnalogIn.h"

using namespace leka;

LKCoreMicrophone micro(PinName::MCU_MIC_INPUT);

float test_set_sound(float value)
{
	spy_AnalogIn_setValue(value);
	return value;
}

TEST(LKCoreMicrophoneTest, initialization)
{
	ASSERT_NE(&micro, nullptr);
}

TEST(LKCoreMicrophoneTest, readMinSound)
{
	auto expected = test_set_sound(0.0f);

	ASSERT_EQ(expected, micro.readVolume());
}

TEST(LKCoreMicrophoneTest, readAverageSound)
{
	auto expected = test_set_sound(0.50f);

	ASSERT_EQ(expected, micro.readVolume());
}

TEST(LKCoreMicrophoneTest, readMaxSound)
{
	auto expected = test_set_sound(1.0f);

	ASSERT_EQ(expected, micro.readVolume());
}
