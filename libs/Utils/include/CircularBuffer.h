// Leka - LekaOS
// Copyright 2021 APF France handicap (based on work by Mbed-OS)
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_CIRCULAR_BUFFER_H_
#define _LEKA_OS_LIB_CIRCULAR_BUFFER_H_

#include <cstdint>

#include "platform/Span.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_atomic.h"
#include "platform/mbed_critical.h"

namespace leka::utils {

template <typename T, uint32_t BufferSize, typename CounterType = uint32_t>
class CircularBuffer
{
  public:
	CircularBuffer()
	{
		static_assert(
			(sizeof(CounterType) >= sizeof(uint32_t)) || (BufferSize < (((uint64_t)1) << (sizeof(CounterType) * 8))),
			"Invalid BufferSize for the CounterType");
	}

	~CircularBuffer() = default;

	void push(const T &data)
	{
		core_util_critical_section_enter();

		_buffer[_head] = data;

		_head = incrementCounter(_head);

		if (_full) {
			_tail = _head;
		} else if (_head == _tail) {
			_full = true;
		}

		core_util_critical_section_exit();
	}

	void push(const T *src, CounterType len)
	{
		if (len <= 0) {
			return;
		}

		core_util_critical_section_enter();

		// if we try to write more bytes than the buffer can hold we only bother writing the last bytes
		if (len > BufferSize) {
			_tail = 0;
			_head = 0;
			_full = true;
			std::copy(src + len - BufferSize, src + len, _buffer.data());
		} else {
			// we need to adjust the tail at the end if we're filling the buffer of overflowing
			bool adjust_tail = ((BufferSize - non_critical_size()) <= len);

			CounterType written = len;

			// on first pass we write as much as we can to the right of head
			if ((_head + written) > BufferSize) {
				written = BufferSize - _head;
			}

			std::copy(src, src + written, _buffer.data() + _head);
			_head = incrementCounter(_head, written);

			// we might need to continue to write from the start of the buffer
			if (CounterType items_left_to_write = len - written) {
				std::copy(src + written, src + written + items_left_to_write, _buffer.data());
				_head = items_left_to_write;
			}

			if (adjust_tail) {
				_tail = _head;
				_full = true;
			}
		}

		core_util_critical_section_exit();
	}

	auto pop(T &data) -> bool
	{
		core_util_critical_section_enter();

		if (non_critical_empty()) {
			core_util_critical_section_exit();
			return false;
		}

		data  = _buffer[_tail];
		_tail = incrementCounter(_tail);
		_full = false;

		core_util_critical_section_exit();

		return true;
	}

	auto pop(T *dest, CounterType len) -> CounterType
	{
		core_util_critical_section_enter();

		if (len <= 0 || non_critical_empty()) {
			core_util_critical_section_exit();
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

		std::copy(_buffer.begin() + _tail, _buffer.begin() + _tail + data_popped, dest);
		_tail = incrementCounter(_tail, data_popped);

		// if we looped over the end we may need to pop again
		if (CounterType items_left_to_pop = len - data_popped) {
			std::copy(_buffer.begin(), _buffer.begin() + items_left_to_pop, dest + data_popped);
			_tail = items_left_to_pop;
			data_popped += items_left_to_pop;
		}

		_full = false;

		core_util_critical_section_exit();

		return data_popped;
	}

	[[nodiscard]] auto empty() const -> bool
	{
		core_util_critical_section_enter();
		bool is_empty = non_critical_empty();
		core_util_critical_section_exit();
		return is_empty;
	}

	[[nodiscard]] auto full() const -> bool { return core_util_atomic_load_bool(&_full); }

	void reset()
	{
		core_util_critical_section_enter();
		_head = 0;
		_tail = 0;
		_full = false;
		core_util_critical_section_exit();
	}

	auto size() const -> CounterType
	{
		core_util_critical_section_enter();
		CounterType elements = non_critical_size();
		core_util_critical_section_exit();
		return elements;
	}

	auto peek(T &data) const -> bool
	{
		core_util_critical_section_enter();
		if (non_critical_empty()) {
			core_util_critical_section_exit();
			return false;
		}

		core_util_critical_section_enter();
		data = _buffer[_tail];
		core_util_critical_section_exit();

		return true;
	}

	auto peekAt(CounterType position, T &data) const -> bool
	{
		core_util_critical_section_enter();

		if (non_critical_empty() || position >= non_critical_size()) {
			core_util_critical_section_exit();
			return false;
		}

		data = _buffer[position];

		core_util_critical_section_exit();

		return true;
	}

  private:
	[[nodiscard]] auto non_critical_empty() const -> bool
	{
		bool is_empty = (_head == _tail) && !_full;
		return is_empty;
	}

	auto non_critical_size() const -> CounterType
	{
		CounterType elements;
		if (!_full) {
			if (_head < _tail) {
				elements = BufferSize + _head - _tail;
			} else {
				elements = _head - _tail;
			}
		} else {
			elements = BufferSize;
		}
		return elements;
	}

	auto incrementCounter(CounterType val, CounterType increment = 1) -> CounterType
	{
		val += increment;

		if (val >= BufferSize) {
			val = 0;
		}

		return val;
	}

	std::array<T, BufferSize> _buffer;
	CounterType _head {0};
	CounterType _tail {0};
	bool _full {false};
};

}	// namespace leka::utils

#endif	 // _LEKA_OS_LIB_CIRCULAR_BUFFER_H_
