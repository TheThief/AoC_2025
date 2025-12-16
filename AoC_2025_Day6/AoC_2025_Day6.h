#pragma once

#include "common/stream_line.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day6
{
	struct puzzle_input_1
	{
		std::vector<std::vector<int>> numbers;
		std::vector<char> operations;

		friend std::istream& operator>>(std::istream& is, puzzle_input_1& input)
		{
			std::vector<std::string> lines = std::ranges::istream_view<line<std::string>>(is) | std::ranges::to<std::vector<std::string>>();

			input.numbers = lines |
				std::views::take(lines.size()-1) |
				std::views::transform(
					[](auto&& l)
					{
						line<std::vector<int>> nums;
						std::ispanstream ss(unwrap(l));
						ss >> nums;
						return unwrap(std::move(nums));
					}) |
				std::ranges::to<std::vector<std::vector<int>>>();

			input.numbers = // transpose
				std::views::iota(0uz, input.numbers[0].size()) |
				std::views::transform(
					[&input](auto i)
					{
						return input.numbers |
							std::views::transform(
								[i](auto& line)
								{
									return line[i];
								});
					}) |
				std::ranges::to<std::vector<std::vector<int>>>();

			line<std::vector<char>> line;
			std::ispanstream ss(lines.back());
			ss >> line;
			input.operations = unwrap(std::move(line));

			return is;
		}
	};

	std::int64_t part1(const puzzle_input_1& input)
	{
		return std::ranges::fold_left(
			std::views::zip_transform(
				[](auto values, char operation)
				{
					if (operation == '+')
					{
						return std::ranges::fold_left(values, (std::int64_t)0, std::plus{});
					}
					else if (operation == '*')
					{
						return std::ranges::fold_left(values, (std::int64_t)1, std::multiplies<>());
					}
					throw std::runtime_error("unknown operation");
				},
				input.numbers,
				input.operations),
			(std::int64_t)0, std::plus{});
	}

	struct puzzle_input_2
	{
		std::vector<std::vector<int>> numbers;
		std::vector<char> operations;

		friend std::istream& operator>>(std::istream& is, puzzle_input_2& input)
		{
			std::vector<std::string> lines = std::ranges::istream_view<line<std::string>>(is) | std::ranges::to<std::vector<std::string>>();

			input.numbers = std::views::iota(0uz, lines[0].size()) |
				std::views::transform(
					[&lines](auto i)
					{
						return lines |
							std::views::take(lines.size() - 1) |
							std::views::transform(
								[i](auto& line)
								{
									return line[i];
								}) |
							std::ranges::to<std::string>();
					}) |
				std::views::split(std::string(lines.size() - 1, ' ')) |
				std::views::transform(
					[](auto&& chunk)
					{
						return chunk |
							std::views::transform(
								[](auto&& col)
								{
									std::string s = col;
									int i;
									std::ispanstream ss(s);
									ss >> i;
									return i;
								}) |
							std::ranges::to<std::vector<int>>();
					}) |
				std::ranges::to<std::vector<std::vector<int>>>();

			line<std::vector<char>> line;
			std::ispanstream ss(lines.back());
			ss >> line;
			input.operations = unwrap(std::move(line));

			return is;
		}
	};

	std::int64_t part2(const puzzle_input_2& input)
	{
		return std::ranges::fold_left(
			std::views::zip_transform(
				[](auto values, char operation)
				{
					if (operation == '+')
					{
						return std::ranges::fold_left(values, (std::int64_t)0, std::plus{});
					}
					else if (operation == '*')
					{
						return std::ranges::fold_left(values, (std::int64_t)1, std::multiplies<>());
					}
					throw std::runtime_error("unknown operation");
				},
				input.numbers,
				input.operations),
			(std::int64_t)0, std::plus{});
	}
}

