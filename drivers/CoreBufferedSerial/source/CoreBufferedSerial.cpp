// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "CoreBufferedSerial.h"

using namespace leka;

auto CoreBufferedSerial::read(char *buffer, ssize_t length) -> ssize_t
{
	return _interface.read(buffer, length);
}

auto CoreBufferedSerial::write(const char *buffer, ssize_t length) -> ssize_t
{
	return _interface.write(buffer, length);
}
