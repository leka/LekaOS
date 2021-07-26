#include "VibrationTemplate.h"

using namespace leka;

VibrationTemplate::VibrationTemplate(float duration, uint32_t frequency, float amplitude, bool smoothTransition)
	: _duration(duration), _frequency(frequency), _amplitude(amplitude), _smoothTransition(smoothTransition)
{
}

auto VibrationTemplate::getFrequency() const -> const uint32_t &
{
	return _frequency;
}

auto VibrationTemplate::getDuration() const -> const float &
{
	return _duration;
}

auto VibrationTemplate::getAmplitude() const -> const float &
{
	return _amplitude;
}

auto VibrationTemplate::isSmooth() const -> const bool &
{
	return _smoothTransition;
}