// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_SPIKE_SHAPE_H_
#define _LEKA_OS_SPIKE_SHAPE_H_

#include <utility>

#include "LogKit.h"

#pragma once

using TreeOutput = uint8_t;
using IDCard	 = uint16_t;

namespace motion {

struct AbstractShape {
	[[nodiscard]] virtual auto getImageName() const -> std::string_view		   = 0;
	virtual auto isItemInArray(std::uint8_t value) -> bool					   = 0;
	[[nodiscard]] virtual auto getIndexOfItem(std::uint8_t item) const -> int  = 0;
	[[nodiscard]] virtual auto getItemAtIndex(int index) const -> std::uint8_t = 0;
	[[nodiscard]] virtual auto getSizeSequence() const -> size_t			   = 0;
	virtual void setId(int id_card)											   = 0;
	[[nodiscard]] virtual auto getId() const -> std::uint16_t				   = 0;
};

template <size_t N>
class Shape : public AbstractShape
{
  public:
	Shape(std::array<TreeOutput, N> array, std::string_view name_image) : _name_image(name_image), _size_sequence(N)
	{
		for (auto i = 0; i < N; ++i) {
			this->_sequence[i] = array[i];
		}
	}

	Shape(std::uint16_t id, std::array<TreeOutput, N> array, std::string_view name_image)
		: _name_image(name_image), _size_sequence(N), _id_card(id)
	{
		for (size_t i = 0; i < N; ++i) {
			this->_sequence[i] = array[i];
		}
	}

	auto isItemInArray(std::uint8_t item) -> bool final
	{
		for (size_t i = 0; i < N; ++i) {
			if (item == this->_sequence[i]) {
				return true;
			}
		}
		return false;
	}

	[[nodiscard]] auto getIndexOfItem(std::uint8_t item) const -> int final
	{
		for (size_t i = 0; i < N; ++i) {
			if (item == this->_sequence[i]) {
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	[[nodiscard]] auto getImageName() const -> std::string_view final { return this->_name_image; }
	auto getSequence() -> std::array<TreeOutput, N> { return this->_sequence; }
	[[nodiscard]] auto getItemAtIndex(int index) const -> std::uint8_t final { return this->_sequence[index]; }
	[[nodiscard]] auto getSizeSequence() const -> size_t final { return this->_size_sequence; }
	[[nodiscard]] auto getId() const -> uint16_t final { return this->_id_card; }
	void setId(int id_card) final { this->_id_card = id_card; }

  private:
	std::array<TreeOutput, N> _sequence;
	size_t _size_sequence {};
	std::string_view _name_image;
	std::uint16_t _id_card {};
};

}	// namespace motion

#endif
