// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

// LCOV_EXCL_START

#include "MagicCard.h"

namespace leka::activity {

struct Shape {
	MagicCard card;
	const char *id;

	static const Shape square;
	static const Shape circle;
	static const Shape triangle;
	static const Shape star;
};

inline constexpr Shape Shape::square   = Shape {MagicCard::shape_square, "0015"};
inline constexpr Shape Shape::circle   = Shape {MagicCard::shape_circle, "0016"};
inline constexpr Shape Shape::triangle = Shape {MagicCard::shape_triangle, "0017"};
inline constexpr Shape Shape::star	   = Shape {MagicCard::shape_star, "0018"};

}	// namespace leka::activity
