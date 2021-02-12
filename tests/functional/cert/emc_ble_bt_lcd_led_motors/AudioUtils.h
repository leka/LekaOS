#include "FileManager.h"

auto audio_pause_duration = std::chrono::seconds(5);

void playSound(AnalogOut *output, FIL *_file)
{
	static const int _n_bytes_to_read = 4;	 // arbitrary
	int16_t _buffer[_n_bytes_to_read] = {0};

	uint32_t _ns_sample_rate		= 22676;				 // 1,000,000,000 / 44,100 (in ns)
	uint32_t ns_sample_rate_adapted = _ns_sample_rate / 6;	 // For some reason unknown, sample rate have to be divided
	uint32_t bytesread				= _n_bytes_to_read;

	/* START READ WAV */
	while (bytesread == _n_bytes_to_read) {
		// Read "_n_bytes_to_read" from file at each iteration. Real bytes read is given by "bytesread"
		if (f_read(_file, &_buffer[0], _n_bytes_to_read, (UINT *)&bytesread) == FR_OK) {
			// Play every 2-bytes (sound encoded in 16 bits)
			for (uint32_t j = 0; j < bytesread; j += 2) {		 // Play one channel, data for stereo are alternate
				output->write_u16((_buffer[j] + 0x8000) >> 2);	 // offset for int16 data (0x8000) and volume 25% (>>2)
				wait_ns(ns_sample_rate_adapted);				 // adjust play speed
			}
		}
	}
	/* END READ WAV*/
}

void playSoundPeriodically(AnalogOut *audio_output)
{
	const char filename[] = "file_example_WAV_5MG.wav";
	FIL file;

	while (true) {
		if (f_open(&file, filename, FA_READ) == FR_OK) {
			playSound(audio_output, &file);
			f_close(&file);
		}
		ThisThread::sleep_for(audio_pause_duration);
	}
}