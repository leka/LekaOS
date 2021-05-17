// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CRITICAL_SECTION_H_
#define _LEKA_OS_LIB_CRITICAL_SECTION_H_

namespace leka {

class CriticalSection
{
  public:
	CriticalSection() = default;

	static void lock();
	static void unlock();
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_CRITICAL_SECTION_H_
