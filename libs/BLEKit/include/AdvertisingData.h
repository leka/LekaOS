// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <cstdint>

namespace leka {

struct AdvertisingData {
	const char *name = "Leka";	 // TODO: Get default name from configuration files
	uint8_t battery {};
	uint8_t is_charging {};
	uint8_t version_major {};
	uint8_t version_minor {};
	uint8_t is_deep_sleeping {};

	auto data()
	{
		updateValues();
		return _internal_values.data();
	}

	[[nodiscard]] auto size() const -> uint32_t { return _internal_values.size(); }

	// private:
	void updateValues()
	{
		_internal_values = {
			battery, is_charging, version_major, version_minor, is_deep_sleeping,
		};
	}

	std::array<uint8_t, 5> _internal_values = {};
};

}	// namespace leka
