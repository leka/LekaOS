#include "VibrationTemplate.h"

using namespace leka;

VibrationTemplate::VibrationTemplate(fseconds duration, uint32_t frequency, float amplitude,
									 VibrationEnvelope::EnvelopType eType, float triangDutyCycle)
	: _duration(duration), _frequency(frequency), _amplitude(amplitude)
{
	if (_duration.count() < 0) {
		log_error("ERROR: Vibration duration can't be negative");
		// TODO() : handle error
	}
	if (_frequency == 0) {
		log_error("ERROR: Vibration freq can't be 0Hz");
		// TODO() : handle error
	}
	if (_amplitude < 0.F || _amplitude > 1.F) {
		log_error("ERROR: Vibration amplitude must be between 0 and 1");
		// TODO() : handle error
	}

	switch (eType) {
		case VibrationEnvelope::EnvelopType::Window:
			this->_envelope = std::make_shared<WindowEnvelope>();
			break;
		case VibrationEnvelope::EnvelopType::Triangle:
			this->_envelope = std::make_shared<TriangleEnvelope>(triangDutyCycle);
			break;
		case VibrationEnvelope::EnvelopType::Smooth:
			this->_envelope = std::make_shared<SmoothEnvelope>();
			break;
		default:
			log_error("ERROR: Envelope Not Implemented");
			this->_envelope = nullptr;
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