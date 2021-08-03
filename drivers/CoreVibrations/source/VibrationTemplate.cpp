#include "VibrationTemplate.h"

using namespace leka;

VibrationTemplate::VibrationTemplate(fseconds duration, uint32_t frequency, float amplitude,
									 VibrationEnvelope::EnvelopType eType, float triangDutyCycle)
	: _duration(duration), _frequency(frequency), _amplitude(amplitude)
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
			printf("Envelope Not Implemented\n\n");
			this->_envelope = nullptr;
			// TODO() : change instantiation when pulse is implemented
			break;
	}
}