// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_TWO_MOTORS_BASE_H_
#define _LEKA_OS_LIB_TWO_MOTORS_BASE_H_

enum class Turn
{
	left,
	right
};

using turn_t = Turn;

enum class Translation
{
	forward,
	backward
};

using translation_t = Translation;

class TwoMotorsBase
{
  public:
	virtual void translate(translation_t translation, float speed) = 0;
	virtual void spin(turn_t turn, float speed)					   = 0;
	virtual void stop(void)										   = 0;
	virtual void brake(void)									   = 0;
};

#endif	 // _LEKA_OS_LIB_TWO_MOTORS_BASE_H_
