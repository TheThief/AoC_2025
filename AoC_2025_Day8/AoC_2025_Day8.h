#pragma once

#include "common/stream_line.h"
#include "common/delimitered.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day8
{
	struct puzzle_input
	{
		std::vector<std::array<int,3>> boxes;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.boxes = std::ranges::istream_view<line<delimitered_array<int, 3, ','>>>(is) | std::ranges::to<std::vector<std::array<int, 3>>>();
			return is;
		}
	};

	double distance(std::array<int, 3> box1, std::array<int, 3> box2)
	{
		return
			(double)(box2[0] - box1[0]) * (box2[0] - box1[0]) +
			(double)(box2[1] - box1[1]) * (box2[1] - box1[1]) +
			(double)(box2[2] - box1[2]) * (box2[2] - box1[2]);
	}

	std::int64_t part1(const puzzle_input& input)
	{
		std::vector<int> box_circuits =
			std::views::iota(0, (int)input.boxes.size()) |
			std::ranges::to<std::vector<int>>();
		std::vector<std::pair<int, int>> box_pairs =
			std::views::cartesian_product(std::views::iota(0, (int)input.boxes.size()), std::views::iota(0, (int)input.boxes.size())) |
			std::views::filter([](std::pair<int, int> p) { return p.first < p.second; }) |
			std::ranges::to<std::vector<std::pair<int, int>>>();
		std::ranges::sort(box_pairs, std::less{},
			[&input](std::pair<int, int> p)
			{
				return distance(input.boxes[p.first], input.boxes[p.second]);
			});

		const int max_iterations = input.boxes.size() < 100 ? 10 : 1000;
		int i = 0;

		for (auto& box_pair : box_pairs)
		{
			if (box_circuits[box_pair.first] != box_circuits[box_pair.second])
			{
				// different circuits, merge
				std::ranges::replace(box_circuits, box_circuits[box_pair.second], box_circuits[box_pair.first]);
			}

			if (++i >= max_iterations)
				break;
		}

		std::ranges::sort(box_circuits);
		std::vector circuit_sizes =
			box_circuits |
			std::views::chunk_by(std::equal_to{}) |
			std::views::transform(
				[](auto&& chunk)
				{
					return chunk.size();
				}) |
			std::ranges::to<std::vector>();
		std::ranges::sort(circuit_sizes, std::greater{});
		return std::ranges::fold_left(circuit_sizes | std::views::take(3), (int64_t)1, std::multiplies{});
	}

	std::int64_t part2(const puzzle_input& input)
	{
		std::vector<int> box_circuits =
			std::views::iota(0, (int)input.boxes.size()) |
			std::ranges::to<std::vector<int>>();
		std::vector<std::pair<int, int>> box_pairs =
			std::views::cartesian_product(std::views::iota(0, (int)input.boxes.size()), std::views::iota(0, (int)input.boxes.size())) |
			std::views::filter([](std::pair<int, int> p) { return p.first < p.second; }) |
			std::ranges::to<std::vector<std::pair<int, int>>>();
		std::ranges::sort(box_pairs, std::less{},
			[&input](std::pair<int, int> p)
			{
				return distance(input.boxes[p.first], input.boxes[p.second]);
			});

		for (auto& box_pair : box_pairs)
		{
			if (box_circuits[box_pair.first] != box_circuits[box_pair.second])
			{
				// different circuits, merge
				std::ranges::replace(box_circuits, box_circuits[box_pair.second], box_circuits[box_pair.first]);

				if (std::ranges::adjacent_find(box_circuits, std::not_equal_to{}) == box_circuits.end())
				{
					// all boxes are connected
					return (int64_t)input.boxes[box_pair.first][0] * input.boxes[box_pair.second][0];
				}
			}
		}

		return -1;
	}
}



