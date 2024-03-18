// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "CoreEventQueue.h"
#include "DigitalOut.h"
#include "FileManagerKit.h"
#include "WavFile.h"
#include "interface/drivers/DAC.h"
#include "interface/drivers/STM32HalBasicTimer.h"

namespace leka {

class AudioKit
{
  public:
	explicit AudioKit(interface::STM32HalBasicTimer &dac_timer, interface::DACDMA &dac)
		: _dac_timer(dac_timer), _dac(dac)
	{
		// nothing to do
	}

	void initialize();

	void enableAudio();
	void disableAudio();

	void play(const std::filesystem::path &path);
	void stop();

	void setData(uint32_t offset);
	void run();

  private:
	mbed::DigitalOut _audio_enable {SOUND_ENABLE, 1};

	interface::STM32HalBasicTimer &_dac_timer;
	interface::DACDMA &_dac;

	FileManagerKit::File _file {};
	WavFile _wav_file {_file};

	CoreEventQueue _event_queue {};

	static constexpr uint32_t played_data_size {2000};
	std::array<uint16_t, played_data_size> played_data {};

	static constexpr uint8_t _repetition {10};
};

}	// namespace leka
