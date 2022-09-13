// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "MagicCard.h"

namespace leka::activity {

struct Emotion {
	std::tuple<MagicCard, MagicCard> cards;
	const char *id;

	static const Emotion fear;
	static const Emotion disgust;
	static const Emotion anger;
	static const Emotion joy;
	static const Emotion sadness;
};

inline constexpr Emotion Emotion::joy =
	Emotion {std::make_tuple(MagicCard::emotion_joy_child, MagicCard::emotion_joy_leka), "004A"};
inline constexpr Emotion Emotion::sadness =
	Emotion {std::make_tuple(MagicCard::emotion_sadness_child, MagicCard::emotion_sadness_leka), "004D"};
inline constexpr Emotion Emotion::fear =
	Emotion {std::make_tuple(MagicCard::emotion_fear_child, MagicCard::emotion_fear_leka), "004F"};
inline constexpr Emotion Emotion::anger =
	Emotion {std::make_tuple(MagicCard::emotion_anger_child, MagicCard::emotion_anger_leka), "0050"};
inline constexpr Emotion Emotion::disgust =
	Emotion {std::make_tuple(MagicCard::emotion_disgust_child, MagicCard::emotion_disgust_leka), "0051"};

}	// namespace leka::activity

// LCOV_EXCL_STOP
