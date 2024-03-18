// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>

#include "interface/drivers/STM32HalBasicTimer.h"

namespace leka {

class CoreSTM32HalBasicTimer : public interface::STM32HalBasicTimer
{
	static constexpr float DEFAULT_AUDIO_FILE_SAMPLE_RATE = 44'100;

  public:
	CoreSTM32HalBasicTimer(interface::STM32Hal &hal);

	[[nodiscard]] auto getHandle() -> TIM_HandleTypeDef & final;

	void registerCallback(std::function<void()> const &callback);
	void linkDACTimer(DAC_ChannelConfTypeDef *config) final;

	void initialize(float frequency = DEFAULT_AUDIO_FILE_SAMPLE_RATE) final;
	void terminate() final;

	void start() final;
	void stop() final;

  private:
	void _registerMspCallbacks();

	interface::STM32Hal &_hal;

	TIM_HandleTypeDef _htim {};

	std::function<void()> _callback {};
};

}	// namespace leka
