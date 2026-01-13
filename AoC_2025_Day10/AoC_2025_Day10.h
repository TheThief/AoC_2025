#pragma once

#include "common/stream_line.h"
#include "common/delimitered.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day10
{
	struct machine
	{
		std::string lights;
		std::vector<std::vector<int>> buttons;
		std::vector<int> joltages;

		friend std::istream& operator>>(std::istream& is, machine& m)
		{
			is >> quoted_value<std::string&, '[', ']'>{ m.lights };
			m.buttons = std::ranges::istream_view<quoted_value<delimitered_vector<int, ','>, '(', ')'>>(is) | std::ranges::to<std::vector<std::vector<int>>>();
			is.clear();
			delimitered_vector<int, ','> joltages;
			is >> quoted_value<delimitered_vector<int, ','>&, '{', '}'>{ joltages };
			m.joltages = std::move(joltages);
			return is;
		}
	};

	struct puzzle_input
	{
		std::vector<machine> machines;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.machines = std::ranges::istream_view<line<machine>>(is) | std::ranges::to<std::vector<machine>>();
			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		return std::ranges::fold_left(
			input.machines |
			std::views::transform(
				[](auto&& m)
				{
					return std::ranges::min(
						std::views::iota(1uz, 1uz << m.buttons.size()) |
						std::views::transform(
							[&m](size_t button_bits)
							{
								std::string lights = std::string(m.lights.length(), '.');
								for (size_t remaining = button_bits, button_index = std::countr_zero(remaining); remaining != 0; remaining &= remaining-1, button_index = std::countr_zero(remaining))
								{
									for (int light_index : m.buttons[button_index])
									{
										lights[light_index] = (lights[light_index] == '.') ? '#' : '.';
									}
								}
								return lights == m.lights ? std::popcount(button_bits) : std::numeric_limits<int64_t>::max();
							}));
				}),
			(std::int64_t)0,
			std::plus{});
	}

	constexpr auto is_positive_even =
		[](std::integral auto n) constexpr noexcept
		{
			return n >= 0 && n % 2 == 0;
		};

	constexpr auto is_zero =
		[](std::integral auto n) constexpr noexcept
		{
			return n == 0;
		};

	int min_presses(const std::vector<std::vector<int>>& buttons, std::vector<int> joltages)
	{
		return std::ranges::min(
			std::views::iota(0uz, 1uz << buttons.size()) |
			std::views::transform(
				[&](size_t button_bits)
				{
					std::vector<int> new_joltages = joltages;
					for (size_t remaining = button_bits, button_index = std::countr_zero(remaining); remaining != 0; remaining &= remaining - 1, button_index = std::countr_zero(remaining))
					{
						for (int joltage_index : buttons[button_index])
						{
							--new_joltages[joltage_index];
							if (new_joltages[joltage_index] < 0)
							{
								return std::numeric_limits<int>::max();
							}
						}
					}

					if (std::ranges::all_of(new_joltages, is_zero))
					{
						return std::popcount(button_bits);
					}
					else if (std::ranges::all_of(new_joltages, is_positive_even))
					{
						std::ranges::for_each(new_joltages, [](int& n) { n /= 2; });
						int presses = min_presses(buttons, new_joltages);
						if (presses == std::numeric_limits<int>::max())
						{
							return std::numeric_limits<int>::max();
						}
						return std::popcount(button_bits) + presses * 2;
					}
					return std::numeric_limits<int>::max();
				}));
	}

	std::int64_t part2(const puzzle_input& input)
	{
		return std::ranges::fold_left(
			input.machines |
			std::views::transform(
				[](auto&& m)
				{
					auto min = min_presses(m.buttons, m.joltages);
					std::println("min presses for machine with lights {} is {}", m.lights, min);
					return min;
				}),
			(std::int64_t)0,
			std::plus{});
	}
}
