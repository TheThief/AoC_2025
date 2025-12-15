#pragma once

import std;

template<typename T>
struct unwrap_result
{
	using type = T;
};
template<typename T>
using unwrap_result_t = typename unwrap_result<T>::type;

template<typename T>
inline T&& unwrap(T&& t)
{
	return std::forward<T>(t);
}
