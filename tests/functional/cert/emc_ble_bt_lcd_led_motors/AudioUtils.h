#include <array>
#include <cstdint>

#include "drivers/AnalogOut.h"
#include "rtos/ThisThread.h"

#include "FileManagerKit.h"

auto audio_pause_duration = std::chrono::seconds(5);

void playSound(mbed::AnalogOut *output, leka::FileManagerKit::File &_file)
{
	static const auto _n_bytes_to_read = int {512};	  // arbitrary
	auto _buffer					   = std::array<uint8_t, _n_bytes_to_read> {0};

	auto _ns_sample_rate		 = uint32_t {22676};		// 1,000,000,000 / 44,100 (in ns)
	auto _ns_sample_rate_adapted = _ns_sample_rate * 1.7;	// arbitrary, 1s in MCU is not exactly 1s in real life
	auto bytesread				 = uint32_t {_n_bytes_to_read};

	/* START READ WAV */
	while (bytesread == _n_bytes_to_read) {
		// Read "_n_bytes_to_read" from file at each iteration. Real bytes read is given by "bytesread"
		if (bytesread = _file.read(_buffer.data(), _n_bytes_to_read); bytesread != 0) {
			// Play every 2-bytes (sound encoded in 16 bits)
			for (uint32_t j = 0; j < bytesread; j += 4) {	// Play one channel, data for stereo are alternate
				output->write_u16((_buffer.at(j + 1) + 0x8000) >>
								  1);	// offset for int16 data (0x8000) and volume 50% (>>1)

				wait_ns(_ns_sample_rate_adapted);	// adjust play speed
			}
		}
	}
	/* END READ WAV*/
}

void playSoundPeriodically(AnalogOut *audio_output)
{
	constexpr auto filename = std::to_array("/fs/sounds/fur-elise-100Hzmin.wav");

	auto file = leka::FileManagerKit::File {};

	while (true) {
		if (auto is_open = file.open(filename.data()); is_open) {
			playSound(audio_output, file);
			file.close();
		}
		rtos::ThisThread::sleep_for(audio_pause_duration);
	}
}
