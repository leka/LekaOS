// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <array>
#include <concepts>
#include <mutex>
#include <span>

#include "platform/mbed_atomic.h"

#include "CriticalSection.h"

namespace leka {

template <typename T, uint32_t BufferSize, typename CounterType = uint32_t>
class CircularQueue
{
  public:
	CircularQueue()
	{
		static_assert((sizeof(CounterType) >= sizeof(uint32_t)) ||
						  (BufferSize < ((static_cast<uint64_t>(1)) << (sizeof(CounterType) * 8))),
					  "Invalid BufferSize for the CounterType");
	}

	~CircularQueue() = default;

	void push(const T &data)
	{
		const std::scoped_lock lock(_lock);

		_buffer[_head] = data;

		_head = incrementCounter(_head);   // LCOV_EXCL_LINE

		if (_full) {
			_tail = _head;
		} else if (_head == _tail) {
			_full = true;
		}
	}

	void push(std::span<const T> items) { push(items.data(), items.size()); }

	void push(const T *src, CounterType len)
	{
		if (len <= 0) {
			return;
		}

		const std::scoped_lock lock(_lock);

		// if we try to write more bytes than the buffer can hold we only bother writing the last bytes
		if (len > BufferSize) {
			_tail = 0;
			_head = 0;
			_full = true;
			std::copy(src + len - BufferSize, src + len, _buffer.data());	// LCOV_EXCL_LINE
		} else {
			// we need to adjust the tail at the end if we're filling the buffer of overflowing
			bool adjust_tail = ((BufferSize - non_critical_size()) <= len);

			CounterType written = len;

			// on first pass we write as much as we can to the right of head
			if ((_head + written) > BufferSize) {
				written = BufferSize - _head;
			}

			std::copy(src, src + written, _buffer.data() + _head);	 // LCOV_EXCL_LINE
			_head = incrementCounter(_head, written);

			// we might need to continue to write from the start of the buffer
			if (CounterType items_left_to_write = len - written) {
				std::copy(src + written, src + written + items_left_to_write, _buffer.data());	 // LCOV_EXCL_LINE
				_head = items_left_to_write;
			}

			if (adjust_tail) {
				_tail = _head;
				_full = true;
			}
		}
	}

	void drop()
	{
		const std::scoped_lock lock(_lock);

		if (non_critical_empty()) {	  // LCOV_EXCL_LINE
			return;
		}

		_tail = incrementCounter(_tail);
		_full = false;
	}

	auto pop(T &data) -> bool
	{
		const std::scoped_lock lock(_lock);

		if (non_critical_empty()) {	  // LCOV_EXCL_LINE
			return false;
		}

		data  = _buffer[_tail];
		_tail = incrementCounter(_tail);
		_full = false;

		return true;
	}

	auto pop(T *dest, CounterType len) -> CounterType
	{
		const std::scoped_lock lock(_lock);

		if (len <= 0 || non_critical_empty()) {
			return 0;
		}

		// make sure we only try to read as much as we have items present
		if (len > non_critical_size()) {
			len = non_critical_size();
		}

		CounterType data_popped = len;

		// items may be split by overlap, take only the number we have to the right of tail
		if ((_tail + data_popped) > BufferSize) {
			data_popped = BufferSize - _tail;
		}

		std::copy(_buffer.begin() + _tail, _buffer.begin() + _tail + data_popped, dest);   // LCOV_EXCL_LINE
		_tail = incrementCounter(_tail, data_popped);

		// if we looped over the end we may need to pop again
		if (auto items_left_to_pop = len - data_popped; items_left_to_pop != 0) {
			std::copy(_buffer.begin(), _buffer.begin() + items_left_to_pop, dest + data_popped);   // LCOV_EXCL_LINE
			_tail = items_left_to_pop;
			data_popped += items_left_to_pop;
		}

		_full = false;

		return data_popped;
	}

	[[nodiscard]] auto empty() const -> bool
	{
		const std::scoped_lock lock(_lock);

		bool is_empty = non_critical_empty();

		return is_empty;
	}

	[[nodiscard]] auto full() const -> bool { return core_util_atomic_load_bool(&_full); }

	void clear()
	{
		const std::scoped_lock lock(_lock);

		_head = 0;
		_tail = 0;
		_full = false;
	}

	auto size() const -> CounterType
	{
		const std::scoped_lock lock(_lock);

		CounterType elements = non_critical_size();	  // LCOV_EXCL_LINE

		return elements;
	}

	auto peek(T &data) const -> bool
	{
		const std::scoped_lock lock(_lock);

		if (non_critical_empty()) {
			return false;
		}

		data = _buffer[_tail];

		return true;
	}

	auto peekAt(CounterType position, T &data) const -> bool
	{
		const std::scoped_lock lock(_lock);

		if (non_critical_empty() || position >= non_critical_size()) {
			return false;
		}

		auto index = position;

		if (_tail + index == BufferSize) {
			index = 0;
		} else if ((_tail + index) > BufferSize - 1) {
			index = BufferSize - _tail;
		} else {
			index = _tail + index;
		}

		data = _buffer[index];

		return true;
	}

	auto hasPattern(std::span<const T> pattern, std::unsigned_integral auto &position) -> bool
	{
		return hasPattern(pattern.data(), pattern.size(), position);
	}

	auto hasPattern(const T *pattern, std::size_t size, std::unsigned_integral auto &position) -> bool
	{
		const std::scoped_lock lock(_lock);

		auto i = 0U;

		while (i <= non_critical_size() - size) {
			auto j = 0U;

			// for current index i, check for pattern match
			for (j = 0; j < size; j++) {
				T d {};
				peekAt(i + j, d);	// LCOV_EXCL_LINE
				if (d != pattern[j]) {
					break;
				}
			}

			if (j == size) {   // if pattern[0...size-1] = _buffer[i, i+1, ...i+size-1]
				position = i;
				return true;
			}

			if (j == 0) {
				i = i + 1;
			} else {
				i = i + j;	 // slide the pattern by j
			}
		}

		return false;
	}

  private:
	[[nodiscard]] auto non_critical_empty() const -> bool
	{
		if (_full || (_head != _tail)) {
			return false;
		}

		return true;
	}

	auto non_critical_size() const -> CounterType
	{
		if (_full) {
			return BufferSize;
		}

		if (_head < _tail) {
			return BufferSize + _head - _tail;
		}

		return _head - _tail;
	}

	auto incrementCounter(CounterType val, CounterType increment = 1) -> CounterType
	{
		val += increment;

		if (val >= BufferSize) {
			val = 0;
		}

		return val;
	}

	std::array<T, BufferSize> _buffer {};
	mutable CriticalSection _lock {};
	CounterType _head {0};
	CounterType _tail {0};
	bool _full {false};
};

}	// namespace leka
