#include "VibrationTemplate.h"

using namespace leka;

VibrationTemplate::VibrationTemplate(fseconds duration, uint32_t frequency, float amplitude, bool smoothTransition)
	: _duration(duration), _frequency(frequency), _amplitude(amplitude), _smoothTransition(smoothTransition)
{
	if (_frequency == 0) {
		printf("Error, freq can't be 0Hz\n");
		// TODO() : handle error
	}
}

auto VibrationTemplate::getFrequency() const -> const uint32_t &
{
	return _frequency;
}

auto VibrationTemplate::getDuration() const -> const fseconds &
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