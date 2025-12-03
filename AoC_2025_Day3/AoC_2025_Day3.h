#pragma once

#include "common/digit.h"
#include "common/stream_line.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day3
{
	struct puzzle_input
	{
		std::vector<std::vector<int>> lines;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.lines = std::ranges::istream_view<line<std::vector<digit>>>(is) | std::ranges::to<std::vector<std::vector<int>>>();

			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		return std::ranges::fold_left(
			input.lines |
			std::views::transform([](const auto& line)
			{
				auto first = std::ranges::max_element(line.begin(), line.end() - 1);
				auto second = std::ranges::max_element(first + 1, line.end());
				return *first * 10 + *second;
			}), 0, std::plus{});
	}

	std::int64_t part2(const puzzle_input& input)
	{
		return std::ranges::fold_left(
			input.lines |
			std::views::transform([](const auto& line)
				{
					constexpr int num = 12;
					auto it = std::ranges::max_element(line.begin(), line.end() - (num - 1));
					std::int64_t total = *it;
					for (int i = num - 2; i >= 0; --i)
					{
						it = std::ranges::max_element(it + 1, line.end() - i);
						total = total * 10 + *it;
					}
					return total;
				}), 0, std::plus{});
	}
}

