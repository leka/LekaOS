// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LL_H_
#define _LEKA_OS_LL_H_

#include <cstdint>

namespace leka {

class CoreLL
{
  public:
	virtual void rawMemoryWrite(uintptr_t destination, uint32_t data)
	{
		// ? NOLINTNEXTLINE - allow reinterpret_cast as there are no alternatives
		*reinterpret_cast<uintptr_t *>(destination) = data;
	}
};

}	// namespace leka

#endif	 // _LEKA_OS_LL_H_
