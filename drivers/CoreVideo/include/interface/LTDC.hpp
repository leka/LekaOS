// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

namespace leka::interface {

class LTDCBase
{
  public:
	virtual ~LTDCBase()		  = default;
	virtual void initialize() = 0;
	virtual LTDC_HandleTypeDef &getHandle();
};

}	// namespace leka::interface
