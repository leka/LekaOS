// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LL_H_
#define _LEKA_OS_LL_H_

#include <cstdint>

namespace leka {

class LKCoreLL
{
  public:
	virtual void rawMemoryWrite(uintptr_t destination, uint32_t data) { *reinterpret_cast<uintptr_t *>(destination) = data; }
};

}	// namespace leka

#endif	 // _LEKA_OS_LL_H_
