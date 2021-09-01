// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_BASE_H_
#define _LEKA_OS_LIB_LTDC_BASE_H_

namespace leka::interface {

class CoreLTDC
{
  public:
	virtual ~CoreLTDC()		  = default;
	virtual void initialize() = 0;
};

}	// namespace leka::interface

#endif	 // _LEKA_OS_LIB_LTDC_BASE_H_
