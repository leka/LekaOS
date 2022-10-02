// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "../SPI.h"

namespace mbed {

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel) : _sw_ssel(mbed::DigitalOut {NC})
{
	// nothing to do
}

SPI::~SPI() = default;

auto SPI::write(int value) -> int
{
	return 0;
}

auto SPI::write(const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length) -> int
{
	leka::spy_spi_written_values.assign(tx_buffer, tx_buffer + tx_length);
	return std::max(tx_length, rx_length);
}

void SPI::lock() {
	// nothing to do
};

void SPI::unlock() {
	// nothing to do
};

}	// namespace mbed

namespace leka {

std::vector<uint8_t> spy_spi_written_values;

auto spy_spi_getValuesAfterWrite() -> std::vector<uint8_t>
{
	return spy_spi_written_values;
}

}	// namespace leka
