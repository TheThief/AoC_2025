#pragma once

#include "common/delimitered.h"
#include "common/stream_line.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day5
{
	struct puzzle_input
	{
		std::vector<std::pair<std::int64_t,std::int64_t>> ranges;
		std::vector<std::int64_t> values;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.ranges =
				std::ranges::istream_view<line<std::string>>(is) |
				std::views::take_while([](auto&& line) { return unwrap(line) != ""; }) |
				std::views::transform(
					[](auto&& line)
					{
						delimitered_pair<std::int64_t, std::int64_t, '-'> p;
						std::ispanstream ss(unwrap(line));
						ss >> p;
						return unwrap(p);
					}) |
				std::ranges::to<std::vector<std::pair<std::int64_t, std::int64_t>>>();

			input.values = std::ranges::istream_view<line<std::int64_t>>(is) | std::ranges::to<std::vector<std::int64_t>>();

			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		return std::ranges::count_if(input.values,
			[&](auto&& value)
			{
				return std::ranges::any_of(input.ranges, [&](auto&& range)
				{
					return value >= range.first && value <= range.second;
				});
			});
	}

	std::int64_t part2(const puzzle_input& input)
	{
		std::int64_t count = 0;
		auto ranges = input.ranges;
		std::ranges::sort(ranges, std::less{}, &std::pair<std::int64_t, std::int64_t>::first);
		for (auto it = ranges.begin(); it != ranges.end();)
		{
			auto it2 = it + 1;
			std::int64_t max_end = it->second;
			for (; it2 != ranges.end() && it2->first <= max_end; ++it2)
			{
				max_end = std::max(max_end, it2->second);
			}
			count += max_end - it->first + 1;
			it = it2;
		}

		return count;
	}
}

