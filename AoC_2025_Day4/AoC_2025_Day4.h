#pragma once

#include "common/stream_line.h"
#include "common/mathutils.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day4
{
	struct puzzle_input
	{
		std::vector<std::vector<char>> lines;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.lines = std::ranges::istream_view<line<std::vector<char>>>(is) | std::ranges::to<std::vector<std::vector<char>>>();
			return is;
		}
	};

	const auto find_removable =
		std::views::transform([](auto&& line)
			{
				return line | std::views::slide(3);
			}) |
		std::views::slide(3) |
		std::views::transform([](auto&& linetriple) // this is just a nasty transpose
			{
				return std::views::zip_transform(
					[](auto triple1, auto triple2, auto triple3)
					{
						return std::array{ triple1, triple2, triple3 };
					},
					linetriple[0],
					linetriple[1],
					linetriple[2]);
			}) |
		std::views::join |
		std::views::filter([](const auto& localgrid)
			{
				return (localgrid[1][1] == '@' &&
					std::ranges::count(localgrid | std::views::join, '@') < 5);
			});
	std::int64_t part1(const puzzle_input& input)
	{
		std::vector<std::vector<char>> grid;
		// this would have been nicer with views::concat
		grid.push_back(std::vector(input.lines[0].size() + 2, '.'));
		for (const auto& line : input.lines)
		{
			std::vector<char> new_line;
			new_line.push_back('.');
			new_line.append_range(line);
			new_line.push_back('.');
			grid.push_back(std::move(new_line));
		}
		grid.push_back(std::vector(input.lines[0].size() + 2, '.'));

		return std::ranges::distance(grid | find_removable);
	}

	std::int64_t part2(const puzzle_input& input)
	{
		std::int64_t count = 0;
		std::vector<std::vector<char>> grid;
		grid.push_back(std::vector(input.lines[0].size() + 2, '.'));
		for (const auto& line : input.lines)
		{
			std::vector<char> new_line;
			new_line.push_back('.');
			new_line.append_range(line);
			new_line.push_back('.');
			grid.push_back(std::move(new_line));
		}
		grid.push_back(std::vector(input.lines[0].size() + 2, '.'));

		std::int64_t localcount = 0;
		do
		{
			auto removable = grid | find_removable |
				std::views::transform([](const auto& localgrid) -> char*
					{
						return &localgrid[1][1];
					}) | std::ranges::to<std::vector<char*>>();
			localcount = std::ranges::distance(removable);
			for (char* c : removable)
			{
				*c = '.';
			}
			count += localcount;
		} while (localcount > 0);
		return count;
	}
}

