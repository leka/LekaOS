// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_LTDC_BASE_H_
#define _LEKA_OS_LIB_LTDC_BASE_H_

namespace leka {

class LKCoreLTDCBase
{
  public:
	virtual ~LKCoreLTDCBase()	  = default;
	virtual void initialize(void) = 0;
	virtual LTDC_HandleTypeDef& getHandle();

};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_LTDC_BASE_H_
