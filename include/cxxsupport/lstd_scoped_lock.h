// Leka - LekaOS
// Copyright 2022 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <concepts>

namespace lstd {

namespace details {
	// Concept for types with static lock/unlock
	template <typename T>
	concept is_static_lock = requires {
		{ T::lock() };
		{ T::unlock() };
	};

	// Concept for types with instance lock/unlock
	template <typename T>
	concept is_instance_lock = requires(T &t) {
		{ t.lock() };
		{ t.unlock() };
	};
}	// namespace details
template <typename T>
class scoped_lock
{
  public:
	explicit scoped_lock(T &lock)
		requires details::is_instance_lock<T>
		: instance_lock(&lock)
	{
		instance_lock->lock();
	}

	explicit scoped_lock()
		requires details::is_static_lock<T>
	{
		T::lock();
	}

	~scoped_lock()
	{
		if constexpr (details::is_static_lock<T>) {
			T::unlock();
		} else if constexpr (details::is_instance_lock<T>) {
			instance_lock->unlock();
		}
	}

	scoped_lock(const scoped_lock &) = delete;
	scoped_lock(scoped_lock &&)		 = delete;

	auto operator=(const scoped_lock &) -> scoped_lock & = delete;
	auto operator=(scoped_lock &&) -> scoped_lock &		 = delete;

  private:
	T *instance_lock {nullptr};
};

}	// namespace lstd
