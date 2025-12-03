#pragma once

#include "common/delimitered.h"
#include "common/stream_required.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day2
{
	struct int_range
	{
		std::int64_t first;
		std::int64_t last;

		friend std::istream& operator>>(std::istream& is, int_range& r)
		{
			is >> r.first >> required{'-'} >> r.last;
			return is;
		}
	};
	struct puzzle_input
	{
		std::vector<int_range> ranges;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.ranges = std::ranges::istream_view<delimitered_value<int_range>>(is) | std::ranges::to<std::vector<int_range>>();

			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		std::int64_t total = 0;
		for (const int_range& r : input.ranges)
		{
			for (std::int64_t i = r.first; i <= r.last; ++i)
			{
				std::string s = std::to_string(i);
				if (std::string_view(s).substr(0, s.size() / 2) == std::string_view(s).substr((s.size() / 2)))
				{
					total += i;
				}
			}
		}
		return total;
	}

	std::int64_t part2(const puzzle_input& input)
	{
		std::int64_t total = 0;
		for (const int_range& r : input.ranges)
		{
			for (std::int64_t i = r.first; i <= r.last; ++i)
			{
				std::string s = std::to_string(i);
				auto is_repeating_pattern = [](std::string_view sv)
					{
						for (int len = 1; len <= sv.size() / 2; ++len)
						{
							if (sv.size() % len == 0)
							{
								for (int repeat = 1; repeat < sv.size() / len; ++repeat)
								{
									if (sv.substr(0, len) != sv.substr(len * repeat, len))
									{
										goto next_len;
									}
								}
								return true;
							}
						next_len:;
						}
						return false;
					};
				if (is_repeating_pattern(s))
					total += i;
			}
		}
		return total;
	}
}
