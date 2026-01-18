#pragma once

import std;

template<typename T, typename Alloc = std::allocator<T>>
std::istream& operator>>(std::istream& is, std::vector<T, Alloc>& v)
{
	v.assign_range(std::ranges::istream_view<T>(is));
	return is;
}
