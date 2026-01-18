#pragma once

import std;

template<typename Func, typename... Args>
std::invoke_result_t<Func, Args...> memoize(Func func, Args... args)
{
	static std::/*unordered_*/map<std::tuple<std::remove_cvref_t<Args>...>, std::remove_cvref_t<std::invoke_result_t<Func, Args...>>> cache;
	auto key = std::make_tuple(args...);
	auto it = cache.find(key);
	if (it != cache.end())
		return it->second;
	auto result = std::invoke(func, args...);
	cache.emplace(std::move(key), std::move(result));
	return result;
}
