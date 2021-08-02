#include "VibrationTemplate.h"

using namespace leka;

VibrationTemplate::VibrationTemplate(fseconds duration, uint32_t frequency, float amplitude,
									 VibrationEnvelope::EnvelopType eType, float triangDutyCycle)
	: _duration(duration),
	  _frequency(frequency),
	  _amplitude(amplitude),
	  _envelope(nullptr),
	  _currentSample(0),
	  _totalSamples(0)
{
	if (_frequency == 0) {
		printf("Error, freq can't be 0Hz\n");
		// TODO() : handle error
	}

	switch (eType) {
		case VibrationEnvelope::Window:
			this->_envelope = std::make_shared<WindowEnvelope>();
			break;
		case VibrationEnvelope::Triangle:
			this->_envelope = std::make_shared<TriangleEnvelope>(triangDutyCycle);
			break;
		case VibrationEnvelope::Smooth:
			this->_envelope = std::make_shared<SmoothEnvelope>();
			break;
		case VibrationEnvelope::Pulse:
			printf("EnvelopeNotImplemented\n\n");
			this->_envelope = nullptr;
			// TODO() : change instantiation when pulse is implemented
			break;
	}
}

auto VibrationTemplate::getDuration() const -> const fseconds &
{
	return _duration;
}

auto VibrationTemplate::getFrequency() const -> const uint32_t &
{
	return _frequency;
}

auto VibrationTemplate::getAmplitude() const -> const float &
{
	return _amplitude;
}

auto VibrationTemplate::getCurrentSample() const -> const uint32_t &
{
	return _currentSample;
}

auto VibrationTemplate::getTotalSamples() const -> const uint32_t &
{
	return _totalSamples;
}

void VibrationTemplate::setCurrentSample(uint32_t currentSample)
{
	_currentSample = currentSample;
}

void VibrationTemplate::setTotalSamples(uint32_t totalSamples)
{
	_totalSamples = totalSamples;
}

void VibrationTemplate::applyCurrentEnvelopeSlice(float *buffer, uint32_t nbSamples) const
{
	_envelope->apply(buffer, nbSamples, _currentSample, _totalSamples);
}