// Leka Project - LekaOS_Explorations
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_STATUS_H_
#define _LEKA_OS_STATUS_H_

namespace leka {
enum class Status
{
	SUCCESS = 0,
	ERROR	= 1,
};
using status_t = Status;
}	// namespace leka

#endif	 // _LEKA_OS_STATUS_H_
