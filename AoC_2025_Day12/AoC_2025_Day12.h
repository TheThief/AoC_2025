#pragma once

import std;

#include "common/stream_line.h"
#include "common/delimitered.h"

using namespace std::string_view_literals;

namespace aoc2025::day12
{
	struct present
	{
		std::array<std::string, 3> shape;

		friend std::istream& operator>>(std::istream& is, present& p)
		{
			int id;
			is >> id >> required{':'};
			for (auto& line : p.shape)
			{
				is >> line;
			}
			return is;
		}
	};
	struct tree
	{
		std::pair<int, int> size;
		std::vector<int> present_counts;

		friend std::istream& operator>>(std::istream& is, tree& t)
		{
			delimitered_pair<int, int, 'x'> size;
			line<std::vector<int>> present_counts;
			is >> size >> required{ ':' } >> present_counts;
			t.size = size;
			t.present_counts = present_counts;
			return is;
		}
	};
	struct puzzle_input
	{
		std::vector<present> presents;
		std::vector<tree> trees;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			// istream_view when used with take still extracts one too many items
			// std::ranges::istream_view<present>(is) | std::views::take(6) | std::ranges::to<std::vector<present>>();
			input.presents = std::views::iota(0,6) | std::views::transform(
				[&](int)
				{
					present p;
					is >> p;
					return p;
				}) | std::ranges::to<std::vector<present>>();
			input.trees = std::ranges::istream_view<tree>(is) | std::ranges::to<std::vector<tree>>();
			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		return std::ranges::count_if(input.trees,
			[&](const tree& t)
			{
				int num_presents = std::ranges::fold_left(t.present_counts, 0, std::plus{});
				int trivial_fit = (t.size.first / 3) * (t.size.second / 3);
				if (num_presents <= trivial_fit)
					return true;

				int total_squares = std::ranges::fold_left(
					std::views::zip_transform(
						[](const present& present, int count)
						{
							return std::ranges::fold_left(present.shape |
								std::views::transform(
									[](const std::string& line) {
										return (int)std::ranges::count(line, '#');
									}),
								0, std::plus{}) * count;
						},
						input.presents, t.present_counts),
					0, std::plus{});
				if (total_squares > t.size.first * t.size.second)
					return false;

				return true; // todo - solve NP-hard packing problem
			});
	}

	std::int64_t part2(const puzzle_input& input)
	{
		return 0;
	}
}
