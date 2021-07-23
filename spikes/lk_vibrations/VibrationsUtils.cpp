#include "VibrationsUtils.h"

using namespace leka;
using namespace std::chrono;

int32_t _endOfVibCounter   = 0;
uint32_t _samplesPerPeriod = 0;
bool _vibrationFinished	   = false;

void playPattern(uint16_t *buffer, uint32_t bufferSize, EnvelopType eType, uint16_t numTimes, CoreDACTimer &dtimer,
				 CoreDAC &dac)
{
	fillBufferWithEnvelop(buffer, bufferSize, eType, 0xFFFF);
	fillEnvelop(buffer, bufferSize, 0xFFFF);

	dac.start(buffer, bufferSize);
	dtimer.start();

	int count = 0;
	// printf("Start of loop\n");
	// while (count < numTimes) {
	// 	if (dac.dmaFlag() == CoreDAC::Cpt) {
	// 		dtimer.stop();
	// 		count++;
	// 		dac.dmaFlag() = CoreDAC::None;
	// 		rtos::ThisThread::sleep_for(500ms);
	// 		dtimer.start();
	// 		// printf("count : %d\n", count);
	// 	}
	// }
	// printf("End of loop\n");
	dtimer.stop();
	dac.stop();
}

void fillBufferWithEnvelop(uint16_t *buffer, uint32_t bufferSize, EnvelopType eType, uint16_t maxVal)
{
	switch (eType) {
		case Click:	  // TODO : code this option

		case Buzz:	 // TODO : code this option
			for (uint32_t i = 0; i < bufferSize; ++i) {
				float tmp = sin(i * M_PI / bufferSize);
				buffer[i] = tmp * maxVal;
			}
			break;
		case Pulse:	  // TODO : code this option

			break;
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

		case Triangular:
			uint32_t riseTime = 2 * bufferSize / 3;
			for (uint32_t i = 0; i < riseTime; ++i) {
				buffer[i] = i * ((double)maxVal / riseTime);
			}
			for (uint32_t i = 0; i < bufferSize - riseTime; ++i) {
				buffer[i + riseTime] = maxVal - i * ((double)maxVal / (bufferSize - riseTime));
			}
			break;
	}
}

void fillEnvelop(uint16_t *buffer, uint32_t bufferSize, uint16_t envelopMaxVal)
{
	std::array<uint16_t, SIN_PERIOD_SIZE> sinPeriod = {0};

	fillBufferWithSinWave(sinPeriod.data(), SIN_PERIOD_SIZE, SIN_FREQUENCY, SAMPLING_RATE, DAC_MAX_VAL, 0x0);

	uint32_t i = 0;
	while (i < bufferSize) {
		for (uint32_t j = 0; j < SIN_PERIOD_SIZE; ++j) {
			double coef = buffer[i] / static_cast<double>(envelopMaxVal);
			buffer[i]	= sinPeriod[j] * coef;
			i++;
		}
	}
}

void halfBufferCallback(DAC_HandleTypeDef *hdac)
{
	_endOfVibCounter -= _samplesPerPeriod;
	if (_endOfVibCounter <= 0) {
		_vibrationFinished = true;
	}
}

void cptBufferCallback(DAC_HandleTypeDef *hdac)
{
	_endOfVibCounter -= _samplesPerPeriod;
	if (_endOfVibCounter <= 0) {
		_vibrationFinished = true;
	}
}

auto createSinWavePeriod(uint16_t *sinBuffer, uint32_t samplesPerPeriod) -> uint32_t
{
	// offset and coef to have a positive sinwave of amplitude 1
	double normalizeCoef   = 0.4;
	double normalizeOffset = 0.45;

	for (uint32_t i = 0; i < samplesPerPeriod; ++i) {
		double tmp = normalizeCoef * sin(i * 2.0 * M_PI / samplesPerPeriod) + normalizeOffset;

		sinBuffer[i] = static_cast<uint16_t>(tmp * UINT16_MAX);
		printf("%d\n", sinBuffer[i]);
	}

	return samplesPerPeriod;
}