// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#include "JPEGImageProperties.hpp"

using namespace leka;

auto JPEGImageProperties::getWidthOffset() const -> uint32_t
{
	auto width_offset = uint32_t {0};

	switch (ChromaSubsampling) {
		case JPEG_420_SUBSAMPLING:
		case JPEG_422_SUBSAMPLING:
			if ((ImageWidth % 16) != 0) {
				width_offset = 16 - (ImageWidth % 16);
			}
			break;
		case JPEG_444_SUBSAMPLING:
			if ((ImageWidth % 8) != 0) {
				width_offset = (ImageWidth % 8);
			}
			break;
		default:
			width_offset = 0;
			break;
	}

	return width_offset;
}
