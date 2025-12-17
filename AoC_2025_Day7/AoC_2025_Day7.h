#pragma once

#include "common/stream_line.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day7
{
	struct puzzle_input
	{
		std::vector<std::string> lines;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.lines = std::ranges::istream_view<line<std::string>>(is) | std::ranges::to<std::vector<std::string>>();
			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		std::vector<std::string> lines = input.lines |
			std::views::transform([](const std::string& line)
			{
				return '.' + line + '.';
			})
			| std::ranges::to<std::vector<std::string>>();

		std::int64_t count = 0;
		std::string state = std::string(lines[0].size(), '.');

		for (const auto& line : lines)
		{
			state = '.' + (std::views::zip_transform(
				[&count](auto line_chars, auto state_chars) mutable
				{
					if (line_chars[1] == 'S')
						return '|';
					if (line_chars[1] == '^' && state_chars[1] == '|')
					{
						++count;
						return '.';
					}
					if ((line_chars[0] == '^' && state_chars[0] == '|') ||
					    (line_chars[2] == '^' && state_chars[2] == '|'))
					{
						return '|';
					}
					return state_chars[1];
				},
				line | std::views::slide(3),
				state | std::views::slide(3)) | std::ranges::to<std::string>())
				+ '.';

			std::println("{}",
				std::views::zip_transform(
					[](auto line_char, auto state_char)
					{
						if (line_char != '.')
							return line_char;
						return state_char;
					},
					line,
					state) | std::ranges::to<std::string>());
		}

		return count;
	}

	std::int64_t part2(const puzzle_input& input)
	{
		std::vector<std::string> lines = input.lines |
			std::views::transform([](const std::string& line)
				{
					return '.' + line + '.';
				})
			| std::ranges::to<std::vector<std::string>>();

		std::vector<std::int64_t> state = std::vector(lines[0].size(), (std::int64_t)0);

		for (const auto& line : lines)
		{
			state = std::views::zip_transform(
				[](auto line_chars, auto states) -> std::int64_t
				{
					if (line_chars[1] == 'S')
						return 1;
					if (line_chars[1] == '^')
						return 0;
					std::int64_t state = states[1];
					if (line_chars[0] == '^')
						state += states[0];
					if (line_chars[2] == '^')
						state += states[2];
					return state;
				},
				line | std::views::slide(3),
				state | std::views::slide(3)) | std::ranges::to<std::vector>();
			state.insert(state.begin(), 0);
			state.push_back(0);

			std::println("{}",
				std::views::zip_transform(
					[](auto line_char, auto state) -> char
					{
						if (line_char != '.')
							return line_char;
						if (state == 0)
							return '.';
						if (state < 10)
							return (char)('0' + state);
						if (state < 36)
							return (char)('A' + (state - 10));
						if (state < 62)
							return (char)('a' + (state - 36));
						return '?';
					},
					line,
					state) | std::ranges::to<std::string>());
		}

		return std::ranges::fold_left(state, (std::int64_t)0, std::plus{});
	}
}


