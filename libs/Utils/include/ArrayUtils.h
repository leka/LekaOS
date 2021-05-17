// Leka - LekaOS
// Copyright 2021 APF France handicap
// SPDX-License-Identifier: Apache-2.0

#ifndef _LEKA_OS_LIB_UTILS_ARRAY_H_
#define _LEKA_OS_LIB_UTILS_ARRAY_H_

#include <array>
#include <functional>

namespace leka::utils {

namespace details {

	template <typename T, size_t N, size_t... I>
	constexpr auto _to_array(T (&arr)[N], std::index_sequence<I...>) -> std::array<std::remove_cv_t<T>, N>
	{
		return {{arr[I]...}};
	}

	template <typename T, size_t N, size_t... I>
	constexpr auto _to_array(T(&&arr)[N], std::index_sequence<I...>) -> std::array<std::remove_cv_t<T>, N>
	{
		return {{std::move(arr[I])...}};
	}

}	// namespace details

template <typename T, size_t N>
constexpr auto to_array(T (&arr)[N])
{
	return details::_to_array(arr, std::make_index_sequence<N>());
}

template <typename T, size_t N>
constexpr auto to_array(T(&&arr)[N])
{
	return details::_to_array(std::move(arr), std::make_index_sequence<N>());
}

}	// namespace leka::utils

#endif	 // _LEKA_OS_LIB_UTILS_ARRAY_H_
