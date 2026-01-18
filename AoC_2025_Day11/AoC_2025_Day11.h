#pragma once

import std;

#include "common/stream_line.h"
#include "common/delimitered.h"
#include "common/memoize.h"

using namespace std::string_view_literals;

namespace aoc2025::day11
{
	struct puzzle_input
	{
		std::unordered_map<std::string, std::vector<std::string>> machines;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.machines = std::ranges::istream_view<line<delimitered_pair<std::string, std::vector<std::string>, ':'>>>(is) |
				std::views::transform([](auto&& line) { return unwrap(line); }) |
				std::ranges::to<std::unordered_map<std::string, std::vector<std::string>>>();
			return is;
		}
	};

	std::int64_t num_paths(const puzzle_input& input, const std::string& current_node, std::string_view dest)
	{
		if (current_node == dest)
			return 1;

		return std::ranges::fold_left(
			input.machines.at(current_node) |
			std::views::transform(
				[&](const auto& next_node) {
					return num_paths(input, next_node, dest);
				}),
			0LL,
			std::plus{});
	}

	std::int64_t part1(const puzzle_input& input)
	{
		return num_paths(input, std::string("you"sv), "out"sv);
	}

	std::int64_t num_paths2(const puzzle_input& input, const std::string& current_node, std::string_view dest, bool visited_dac = false, bool visited_fft = false)
	{
		if (current_node == dest)
			return visited_dac && visited_fft;
		if (current_node == "dac"sv)
			visited_dac = true;
		if (current_node == "fft"sv)
			visited_fft = true;

		return std::ranges::fold_left(
			input.machines.at(current_node) |
			std::views::transform(
				[&](const auto& next_node) {
					return memoize([&input](auto&&... args) { return num_paths2(input, args...); }, next_node, dest, visited_dac, visited_fft);
				}),
			0LL,
			std::plus{});
	}

	std::int64_t part2(const puzzle_input& input)
	{
		return num_paths2(input, std::string("svr"sv), "out"sv);
	}
}
