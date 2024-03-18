// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <span>

#include "BehaviorKit.h"
#include "Utils.h"
#include "interface/Command.h"

namespace leka {

struct BehaviorCommand : interface::Command {
	explicit BehaviorCommand(BehaviorKit &kit) : _behaviorkit(kit) {}

	auto id() -> uint8_t override { return cmd::id; }

	auto data() -> uint8_t * override
	{
		args = {};
		return args.data();
	}

	[[nodiscard]] auto size() const -> std::size_t override { return std::size(args); }

	auto execute() -> bool override
	{
		auto [id, chcksm] = std::tuple_cat(args);

		auto expected = [&] { return utils::math::checksum8(std::span {args.data(), args.size() - 1}); };

		if (chcksm != expected()) {
			return false;
		}

		switch (id) {
			case cmd::behavior::stop:
				_behaviorkit.stop();
				break;
			case cmd::behavior::launching:
				_behaviorkit.launching();
				break;
			case cmd::behavior::sleeping:
				_behaviorkit.sleeping();
				break;
			case cmd::behavior::waiting:
				_behaviorkit.waiting();
				break;
			case cmd::behavior::blink_on_charge:
				_behaviorkit.blinkOnCharge();
				break;
			case cmd::behavior::low_battery:
				_behaviorkit.lowBattery();
				break;
			case cmd::behavior::charging_empty:
				_behaviorkit.chargingEmpty();
				break;
			case cmd::behavior::charging_low:
				_behaviorkit.chargingLow();
				break;
			case cmd::behavior::charging_medium:
				_behaviorkit.chargingMedium();
				break;
			case cmd::behavior::charging_high:
				_behaviorkit.chargingHigh();
				break;
			case cmd::behavior::charging_full:
				_behaviorkit.chargingFull();
				break;
			case cmd::behavior::ble_connection_without_video:
				_behaviorkit.bleConnectionWithoutVideo();
				break;
			case cmd::behavior::ble_connection_with_video:
				_behaviorkit.bleConnectionWithVideo();
				break;
			case cmd::behavior::working:
				_behaviorkit.working();
				break;
			case cmd::behavior::file_exchange:
				_behaviorkit.fileExchange();
				break;
			default:
				_behaviorkit.stop();
				break;
		}

		return true;
	}

  private:
	struct cmd {
		static constexpr auto id   = uint8_t {0x60};
		static constexpr auto size = uint8_t {1 + 1};	// id + page + Checksum

		struct behavior {
			static constexpr auto stop						   = uint8_t {0x00};
			static constexpr auto launching					   = uint8_t {0x01};
			static constexpr auto sleeping					   = uint8_t {0x02};
			static constexpr auto waiting					   = uint8_t {0x03};
			static constexpr auto blink_on_charge			   = uint8_t {0x04};
			static constexpr auto low_battery				   = uint8_t {0x05};
			static constexpr auto charging_empty			   = uint8_t {0x06};
			static constexpr auto charging_low				   = uint8_t {0x07};
			static constexpr auto charging_medium			   = uint8_t {0x08};
			static constexpr auto charging_high				   = uint8_t {0x09};
			static constexpr auto charging_full				   = uint8_t {0x0A};
			static constexpr auto ble_connection_without_video = uint8_t {0x0B};
			static constexpr auto ble_connection_with_video	   = uint8_t {0x0C};
			static constexpr auto working					   = uint8_t {0x0D};
			static constexpr auto file_exchange				   = uint8_t {0x0E};
		};
	};

	std::array<uint8_t, cmd::size> args {};
	BehaviorKit &_behaviorkit;
};

}	// namespace leka
