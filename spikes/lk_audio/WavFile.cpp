#include "WavFile.h"

// {
// 	log_info("Header: ");
// 	rtos::ThisThread::sleep_for(100ms);
// 	for (auto &elem: header_array) {
// 		printf("%x ", elem);
// 	}
// 	printf("\n");

// 	rtos::ThisThread::sleep_for(1s);

// 	file.read(data_file);
// 	log_info("Content: ");
// 	rtos::ThisThread::sleep_for(1s);
// 	for (auto i = 0; i < data_file_size; i++) {
// 		int16_t file_value = data_file.at(i);
// 		printf("At %3i: %d\n", i * 2 + 44, file_value);
// 		// uint16_t normalized_value = static_cast<uint16_t>(data_file.at(i) + 0x8000) >> 4;
// 		// printf("%x ", normalized_value);
// 	}
// 	printf("\n");

// 	rtos::ThisThread::sleep_for(1s);
// 	return 0;
// } // Normalization

// {
// 	setData(0);
// 	log_info("Data play (first half loaded):");
// 	rtos::ThisThread::sleep_for(1s);
// 	for (auto i = 0; i < data_play.size(); i++) {
// 		printf("At %3i: %x\n", i * 2 + 44, data_play.at(i));
// 	}
// 	printf("\n");

// 	setData(size / 2);
// 	log_info("Data play (second half loaded): ");
// 	rtos::ThisThread::sleep_for(1s);
// 	for (auto i = 0; i < data_play.size(); i++) {
// 		printf("At %3i: %x\n", i * 2 + 44, data_play.at(i));
// 	}
// 	printf("\n");

// setData(0);
// log_info("Data play (next chunk): ");
// rtos::ThisThread::sleep_for(1s);
// for (auto i = 0; i < data_play.size() / 2; i++) {
// 	printf("At %3i: %x\n", i * 2 + 44 + size * 2, data_play.at(i));
// }
// printf("\n");

// 	rtos::ThisThread::sleep_for(1s);
// 	return 0;
// }	// Correctly filled
