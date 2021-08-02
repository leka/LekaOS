// Leka - LekaOS
// Copyright 2020 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_VIBRATION_TEMPLATE_H_
#define _LEKA_OS_LIB_VIBRATION_TEMPLATE_H_

#include <chrono>
#include <cstdio>
#include <memory>

#include "VibrationEnvelope.h"

namespace leka {

// TODO(max): replace these redefines somewhere else
using fmilliseconds = std::chrono::duration<float, std::milli>;
using fseconds		= std::chrono::duration<float>;
using fminutes		= std::chrono::duration<float, std::ratio<60>>;
/*Redefines taken at : https://philippegroarke.com/posts/2018/chrono_for_humans/
Allow to easily work with durations
*/

class VibrationTemplate
{
  public:
	VibrationTemplate(fseconds duration, uint32_t frequency, float amplitude, VibrationEnvelope::EnvelopType eType,
					  float triangDutyCycle = 1.F);

	[[nodiscard]] auto getDuration() const -> const fseconds &;
	[[nodiscard]] auto getFrequency() const -> const uint32_t &;
	[[nodiscard]] auto getAmplitude() const -> const float &;
	[[nodiscard]] auto getCurrentSample() const -> const uint32_t &;
	[[nodiscard]] auto getTotalSamples() const -> const uint32_t &;

	[[nodiscard]] auto isSmooth() const -> const bool &;

	void setCurrentSample(uint32_t currentSample);
	void setTotalSamples(uint32_t totalSamples);

	void applyCurrentEnvelopeSlice(float *buffer, uint32_t nbSamples) const;

  private:
	fseconds _duration;
	uint32_t _frequency;
	float _amplitude;
	std::shared_ptr<VibrationEnvelope> _envelope;

	uint32_t _currentSample;
	uint32_t _totalSamples;
};

}	// namespace leka

#endif	 // _LEKA_OS_LIB_VIBRATION_TEMPLATE_H_
