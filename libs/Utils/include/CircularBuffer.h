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
	CircularBuffer() : _head(0), _tail(0), _full(false)
	{
		static_assert(
			(sizeof(CounterType) >= sizeof(uint32_t)) || (BufferSize < (((uint64_t)1) << (sizeof(CounterType) * 8))),
			"Invalid BufferSize for the CounterType");
	}

	~CircularBuffer() {}

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
		MBED_ASSERT(len > 0);

		core_util_critical_section_enter();

		/* if we try to write more bytes than the buffer can hold we only bother writing the last bytes */
		if (len > BufferSize) {
			_tail = 0;
			_head = 0;
			_full = true;
			std::copy(src + len - BufferSize, src + len, _buffer);
		} else {
			/* we need to adjust the tail at the end if we're filling the buffer of overflowing */
			bool adjust_tail = ((BufferSize - non_critical_size()) <= len);

			CounterType written = len;

			/* on first pass we write as much as we can to the right of head */
			if ((_head + written) > BufferSize) {
				written = BufferSize - _head;
			}

			std::copy(src, src + written, _buffer + _head);
			_head = incrementCounter(_head, written);

			CounterType left_to_write = len - written;

			/* we might need to continue to write from the start of the buffer */
			if (left_to_write) {
				std::copy(src + written, src + written + left_to_write, _buffer);
				_head = left_to_write;
			}

			if (adjust_tail) {
				_tail = _head;
				_full = true;
			}
		}

		core_util_critical_section_exit();
	}

	void push(mbed::Span<const T> src) { push(src.data(), src.size()); }

	bool pop(T &data)
	{
		bool data_popped = false;

		core_util_critical_section_enter();

		if (!non_critical_empty()) {
			data_popped = true;

			data  = _buffer[_tail];
			_tail = incrementCounter(_tail);
			_full = false;
		}

		core_util_critical_section_exit();

		return data_popped;
	}

	CounterType pop(T *dest, CounterType len)
	{
		MBED_ASSERT(len > 0);

		if (len == 0) {
			return 0;
		}

		CounterType data_popped = 0;

		core_util_critical_section_enter();

		if (!non_critical_empty()) {
			/* make sure we only try to read as much as we have items present */
			if (len > non_critical_size()) {
				len = non_critical_size();
			}
			data_popped = len;

			/* items may be split by overlap, take only the number we have to the right of tail */
			if ((_tail + data_popped) > BufferSize) {
				data_popped = BufferSize - _tail;
			}

			std::copy(_buffer + _tail, _buffer + _tail + data_popped, dest);
			_tail = incrementCounter(_tail, data_popped);

			/* if we looped over the end we may need to pop again */
			CounterType left_to_pop = len - data_popped;

			if (left_to_pop) {
				std::copy(_buffer, _buffer + left_to_pop, dest + data_popped);
				_tail = left_to_pop;

				data_popped += left_to_pop;
			}

			_full = false;
		}

		core_util_critical_section_exit();

		return data_popped;
	}

	mbed::Span<T> pop(mbed::Span<T> dest)
	{
		CounterType popped = pop(dest.data(), dest.size());
		return mbed::make_Span(dest.data(), popped);
	}

	bool empty() const
	{
		core_util_critical_section_enter();
		bool is_empty = non_critical_empty();
		core_util_critical_section_exit();
		return is_empty;
	}

	bool full() const { return core_util_atomic_load_bool(&_full); }

	void reset()
	{
		core_util_critical_section_enter();
		_head = 0;
		_tail = 0;
		_full = false;
		core_util_critical_section_exit();
	}

	CounterType size() const
	{
		core_util_critical_section_enter();
		CounterType elements = non_critical_size();
		core_util_critical_section_exit();
		return elements;
	}

	bool peek(T &data) const
	{
		bool data_updated = false;
		core_util_critical_section_enter();
		if (!empty()) {
			data		 = _buffer[_tail];
			data_updated = true;
		}
		core_util_critical_section_exit();
		return data_updated;
	}

  private:
	bool non_critical_empty() const
	{
		bool is_empty = (_head == _tail) && !_full;
		return is_empty;
	}

	CounterType non_critical_size() const
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

	CounterType incrementCounter(CounterType val, CounterType increment = 1)
	{
		val += increment;

		MBED_ASSERT(val <= BufferSize);

		if (val == BufferSize) {
			val = 0;
		}

		return val;
	}

  private:
	T _buffer[BufferSize];
	CounterType _head;
	CounterType _tail;
	bool _full;
};

}	// namespace leka::utils

#endif	 // _LEKA_OS_LIB_CIRCULAR_BUFFER_H_
