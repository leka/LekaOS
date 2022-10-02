// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "gmock/gmock.h"
#include "interface/drivers/Network.h"

namespace leka::mock {

class Network : public interface::Network
{
  public:
	MOCK_METHOD(bool, connect, (const char *, const char *), (override));
	MOCK_METHOD(HttpResponse &, sendRequest,
				(const char *, const char *, const std::function<void(const char *at, uint32_t length)> &), (override));
};

}	// namespace leka::mock
