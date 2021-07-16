// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "drivers/BufferedSerial.h"
#include "rtos/ThisThread.h"

#include "CoreAudio.h"
#include "HelloWorld.h"
#include "LKCoreSTM32Hal.h"
#include "VibrationsUtils.h"

using namespace leka;
using namespace std::chrono;

// uint16_t _outBuffer[OUT_BUFF_SIZE] = {0};

std::array<uint16_t, OUT_BUFF_SIZE> _outBuffer;
auto serial = mbed::BufferedSerial(USBTX, USBRX, 115200);

LKCoreSTM32Hal hal;
CoreDACTimer coreDACTimer(hal);
CoreDAC coreDAC(hal);
CoreAudio coreAudio(hal, coreDAC, coreDACTimer);

/**
 * @brief This function handles DMA1 stream5 global interrupt.
 */
extern "C" {
void DMA1_Stream5_IRQHandler()
{
	HAL_DMA_IRQHandler(coreDAC.getHandle().DMA_Handle1);
}
}

auto main() -> int
{
	while (true) {
	}
}
