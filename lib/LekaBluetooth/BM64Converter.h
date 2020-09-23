/**
 * @file BM64Converter.h
 * @author Yann Locatelli
 *
 * @version 0.1
 * @date 2020-09-23
 *
 * @copyright Copyright (c) 2020
 */
#ifndef _LEKA_OS_LIB_BM64_CONVERTER_H_
#define _LEKA_OS_LIB_BM64_CONVERTER_H_

#include "mbed.h"

namespace BM64 {
	namespace Command {
		const uint8_t pairing[3]	= {0x02, 0x00, 0x50};
		const size_t pairing_length = 3;

	};	 // namespace Command
	// using command_t = Command;

	namespace Response {
		const uint8_t acknowledge[2]	= {0xFF, 0xFF};	  // Not done yet
		const size_t acknowledge_length = 2;
	};	 // namespace Response
	// using response_t = Response;

	size_t getCommand(const uint8_t *cmd, const size_t cmd_length, uint8_t *buffer);
	// void convertResponse();	  // Events in datasheet

};	 // namespace BM64

#endif
