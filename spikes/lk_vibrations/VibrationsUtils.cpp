#include "VibrationsUtils.h"

// Fill buffer with sin wave at given frequency and sampling rate
// Values can be limited between maxValue and minValue
void fillBufferWithSinWave(uint16_t *buffer, uint32_t bufferSize, uint32_t frequency, uint32_t samplingRate,
						   uint16_t maxValue, uint16_t minValue)
{
	uint32_t samplesPerPeriod = (samplingRate / frequency);

	for (uint32_t i = 0; i < bufferSize; ++i) {
		float tmp = 0.5 * sin(i * 2.0 * M_PI / samplesPerPeriod) + 0.5;
		tmp *= maxValue - minValue;
		buffer[i] = tmp + minValue;
	}
}

void fillBufferWithEnvelop(uint16_t *buffer, uint32_t bufferSize, EnvelopType eType, uint16_t maxVal)
{
	switch (eType) {
		case Click:	  // TODO : code this option
		case Buzz:	  // TODO : code this option
		case Pulse:	  // TODO : code this option
		case Window:
			for (uint32_t i = 0; i < bufferSize; ++i) {
				buffer[i] = maxVal;
			}
			break;

		case Sawtooth:
			for (uint32_t i = 0; i < bufferSize; ++i) {
				buffer[i] = i * ((double)maxVal / bufferSize);
			}
			break;

		case Ramp:
			uint32_t riseTime = 2 * bufferSize / 3;
			for (uint32_t i = 0; i < riseTime; ++i) {
				buffer[i] = i * ((double)maxVal / bufferSize);
			}
			for (uint32_t i = riseTime; i < bufferSize; ++i) {
				buffer[i] = maxVal - i * ((double)maxVal / (bufferSize - riseTime));
			}
			break;
	}
}

void fillEnvelop(uint16_t *buffer, uint32_t bufferSize, uint16_t envelopMaxVal)
{
	std::array<uint16_t, SIN_PERIOD_SIZE> sinPeriod = {0};

	fillBufferWithSinWave(sinPeriod.begin(), SIN_PERIOD_SIZE, SIN_FREQUENCY, SAMPLING_RATE, DAC_MAX_VAL, 0x0);

	uint32_t i = 0;
	while (i < bufferSize) {
		for (uint32_t j = 0; j < SIN_PERIOD_SIZE; ++j) {
			double coef = buffer[i] / static_cast<double>(envelopMaxVal);
			buffer[i]	= sinPeriod[j] * coef;
			i++;
		}
	}
}