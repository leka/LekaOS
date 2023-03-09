// Leka - LekaOS
// Copyright 2023 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/libs/IMUKit.hpp"

namespace leka::mock {

class IMUKit : public interface::IMUKit
{
  public:
	MOCK_METHOD(void, start, (), (override));
	MOCK_METHOD(void, stop, (), (override));
	MOCK_METHOD(void, setOrigin, (), (override));
	MOCK_METHOD(EulerAngles, getEulerAngles, (), (const, override));

	void onEulerAnglesReady(angles_ready_callback_t const &cb) override { angles_ready_callback = cb; }

	void call_angles_ready_callback(const EulerAngles &data) { angles_ready_callback(data); }

  private:
	angles_ready_callback_t angles_ready_callback {};
};

}	// namespace leka::mock
