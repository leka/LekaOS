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
	stop();

	if (auto data_preloaded = preloadData(path); not(data_preloaded)) {
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
	rtos::ThisThread::sleep_for(1ms);
}

auto AudioKit::preloadData(const std::filesystem::path &filename) -> bool
{
	const std::filesystem::path directory {"/fs/home/wav"};
	const std::string extension {".wav"};

	auto path = directory / filename;
	path += extension;

	auto is_open = _file.open(path);   // path not correct
	if (not(is_open)) {
		return false;
	}
	_file_size = _file.size();

	std::array<uint8_t, 44> header {};
	_file.read(header);

	_flash.erase();
	_flash_address = 0;

	auto address				   = uint32_t {0x0};
	constexpr auto flash_page_size = 0x100;
	auto buffer					   = std::array<uint8_t, flash_page_size> {};

	while (auto bytes_read = _file.read(buffer)) {
		_flash.write(address, buffer, bytes_read);
		address += bytes_read;
	}

	_file.close();

	return true;
}

void AudioKit::setData(uint32_t offset)
{
	if (_flash_address > _file_size) {
		stop();
		return;
	}

	auto *start_played_data = played_data.begin() + offset;

	constexpr auto flash_data_size = played_data_size / _repetition / 2;   // /2 half
	auto flash_data = std::array<uint8_t, flash_data_size * 2> {};		   // *2 bytes for 2 bytes to short conversion
	auto *data_p	= reinterpret_cast<int16_t *>(flash_data.begin());

	_flash.read(_flash_address, flash_data, flash_data.size());
	_flash_address += flash_data.size();

	for (uint32_t i = 0; i < flash_data_size; i++) {
		auto normalized_value = static_cast<uint16_t>((*(data_p + i) + 0x8000) >> 4);
		std::fill_n(start_played_data + i * _repetition, _repetition, normalized_value);
	}

	rtos::ThisThread::sleep_for(3ms);	// Related to played_data_size and _repetition
}

void AudioKit::run()
{
	_dac.start();
}
