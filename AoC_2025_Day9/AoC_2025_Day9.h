#pragma once

#include "common/stream_line.h"
#include "common/delimitered.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day9
{
	struct puzzle_input
	{
		std::vector<std::array<int,2>> red_tiles;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.red_tiles = std::ranges::istream_view<line<delimitered_array<int, 2, ','>>>(is) | std::ranges::to<std::vector<std::array<int, 2>>>();
			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		return std::ranges::max(
			std::views::cartesian_product(input.red_tiles, input.red_tiles) |
			std::views::transform([](const auto& pair)
				{
					const auto& [a, b] = pair;
					return (int64_t)(std::abs(b[0] - a[0]) + 1) * (std::abs(b[1] - a[1]) + 1);
				}));
	}

	std::int64_t part2(const puzzle_input& input)
	{
		auto edges = std::views::repeat(input.red_tiles) | std::views::join | std::views::take(input.red_tiles.size() + 1) | std::views::adjacent<2>;
		return std::ranges::max(
			std::views::cartesian_product(input.red_tiles, input.red_tiles) |
			std::views::filter([&edges](const auto& pair)
				{
					const auto& [a, b] = pair;
					return std::ranges::all_of(edges,
						[&a,&b](const auto& edge)
						{
							return
								std::get<0>(edge)[0] <= std::min(a[0], b[0]) && std::get<1>(edge)[0] <= std::min(a[0], b[0]) ||
								std::get<0>(edge)[0] >= std::max(a[0], b[0]) && std::get<1>(edge)[0] >= std::max(a[0], b[0]) ||
								std::get<0>(edge)[1] <= std::min(a[1], b[1]) && std::get<1>(edge)[1] <= std::min(a[1], b[1]) ||
								std::get<0>(edge)[1] >= std::max(a[1], b[1]) && std::get<1>(edge)[1] >= std::max(a[1], b[1]);
						});
				}) |
			std::views::transform([](const auto& pair)
				{
					const auto& [a, b] = pair;
					return (int64_t)(std::abs(b[0] - a[0]) + 1) * (std::abs(b[1] - a[1]) + 1);
				}));
	}
}




