// Leka - LekaOS
// Copyright 2022 APF France handicap
// Based on work by @gurki - https://github.com/gurki/vivid
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <glm/vec3.hpp>

namespace leka::ColorKit::internal {

using col_t = glm::vec<3, float>;

//  main color spaces

struct rgb_t : public col_t {
	rgb_t() = default;
	rgb_t(const float r, const float g, const float b) : col_t(r, g, b) {}
};

struct oklab_t : public col_t {
	oklab_t() = default;
	oklab_t(const float r, const float g, const float b) : col_t(r, g, b) {}

	explicit oklab_t(const col_t &col) : col_t(col) {}
};

struct lrgb_t : public rgb_t {
	using rgb_t::rgb_t;
};

}	// namespace leka::ColorKit::internal
