// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LKCALCULATORKIT_H_
#define _LEKA_OS_LIB_LKCALCULATORKIT_H_

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

#endif	 // _LEKA_OS_LIB_LKCALCULATORKIT_H_
