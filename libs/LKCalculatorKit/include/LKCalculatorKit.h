// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka {

class LKCalculatorKit
{
  public:
	LKCalculatorKit() = default;
	int add(int x, int y);
	int multiply(int x, int y);
	int getLastResult(void);

  private:
	int _last_result = 0;
};

}	// namespace leka
