// Leka - LekaOS
// Copyright 2024 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "AudioKit.h"

#include "rtos/ThisThread.h"

#include "LogKit.h"

using namespace leka;
using namespace std::chrono_literals;

void AudioKit::initialize()
{
	_event_queue.dispatch_forever();

	auto onHalfTransfer		= [this] { setData(0); };
	auto onCompleteTransfer = [this] { setData(played_data_size / 2); };

	_dac.registerDMACallbacks([this, onHalfTransfer] { _event_queue.call(onHalfTransfer); },
							  [this, onCompleteTransfer] { _event_queue.call(onCompleteTransfer); });
	_dac.registerDataToPlay(played_data);

	constexpr auto file_sample_rate = float {44'100.0};
	constexpr auto timer_rate		= float {file_sample_rate * _repetition};

	_dac_timer.initialize(timer_rate);
	_dac.initialize();
}

void AudioKit::enableAudio()
{
	_audio_enable = 1;
}

void AudioKit::disableAudio()
{
	stop();
	_audio_enable = 0;
}

void AudioKit::play(const std::filesystem::path &path)
{
	auto is_open = _wav_file.open(path);
	if (!is_open) {
		return;
	}

	setData(0);
	setData(played_data_size / 2);

	enableAudio();

	run();
}

void AudioKit::stop()
{
	_dac.stop();
}

void AudioKit::setData(uint32_t offset)
{
	if (_wav_file.isEndOfFile()) {
		stop();
		return;
	}

	constexpr auto file_data_size = played_data_size / _repetition / 2;
	auto file_data				  = std::array<int16_t, file_data_size> {};

	_wav_file.read(file_data);

	for (uint32_t i = 0; i < file_data.size(); i++) {
		auto normalized_value = static_cast<uint16_t>((file_data.at(i) + 0x8000) >> 4);
		std::fill_n(played_data.begin() + offset + i * _repetition, _repetition, normalized_value);
	}

	rtos::ThisThread::sleep_for(2ms);	// Related to played_data_size and _repetition
}

void AudioKit::run()
{
	_dac.start();
}
