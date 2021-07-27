// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_VIBRATION_TEMPLATE_H_
#define _LEKA_OS_LIB_VIBRATION_TEMPLATE_H_

#include <chrono>
#include <cstdint>
#include <cstdio>

// enum EnvelopType
// {
// 	Window,
// 	Sawtooth,
// 	Click,		  // small fast
// 	Triangular,	  // Up 2/3 of the time and down 1/3
// 	Buzz,		  // round
// 	Pulse		  // multiple maximums
// };

namespace leka {

// TODO : replace these redefines somewhere else
using fmilliseconds = std::chrono::duration<float, std::milli>;
using fseconds		= std::chrono::duration<float>;
using fminutes		= std::chrono::duration<float, std::ratio<60>>;
/*Redefines taken at : https://philippegroarke.com/posts/2018/chrono_for_humans/
Allow to easily work with durations
*/

class VibrationTemplate
{
  public:
	VibrationTemplate(fseconds duration, uint32_t frequency, float amplitude, bool smoothTransition);

	[[nodiscard]] auto getFrequency() const -> const uint32_t &;
	[[nodiscard]] auto getDuration() const -> const fseconds &;
	[[nodiscard]] auto getAmplitude() const -> const float &;
	[[nodiscard]] auto isSmooth() const -> const bool &;

  private:
	fseconds _duration;
	uint32_t _frequency;
	float _amplitude;
	bool _smoothTransition;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_TEMPLATE_H_
