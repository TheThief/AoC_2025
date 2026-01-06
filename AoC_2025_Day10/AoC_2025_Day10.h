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
						std::views::iota(0uz, 1uz << m.buttons.size()) |
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

	int min_presses(const std::vector<std::vector<int>>& buttons, std::vector<int> joltages)
	{
		auto min_it = std::ranges::min_element(joltages, std::less{},
			[](int i)
			{
				return i != 0 ? i : std::numeric_limits<int>::max();
			});
		auto min_index = std::distance(joltages.begin(), min_it);
		auto min_value = *min_it;
		if (min_value == 0)
		{
			return 0;
		}

		auto valid_buttons = buttons | std::views::filter(
			[&joltages, min_index](const auto& button)
			{
				return
					std::ranges::contains(button, min_index) &&
					std::ranges::all_of(button,
						[&joltages](int i)
						{
							return joltages[i] > 0;
						});
			});

		if (std::ranges::empty(valid_buttons))
		{
			return std::numeric_limits<int>::max();
		}

		return std::ranges::min(
			valid_buttons |
			std::views::transform(
				[&](const auto& button)
				{
					std::vector<int> new_joltages = joltages;
					for (int j : button)
					{
						new_joltages[j] -= min_value;
					}
					int presses = min_presses(buttons, new_joltages);
					if (presses == std::numeric_limits<int>::max())
					{
						return std::numeric_limits<int>::max();
					}
					return presses + min_value;
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





