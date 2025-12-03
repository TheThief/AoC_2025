#pragma once

#include "common/stream_line.h"
#include "common/mathutils.h"

import std;

using namespace std::string_view_literals;

namespace aoc2025::day1
{
	enum class dir
	{
		L,
		R,
	};
	struct movement
	{
		dir d;
		int dist;

		friend std::istream& operator>>(std::istream& is, movement& m)
		{
			char c = 0;
			if (is >> c)
			{
				switch (c)
				{
				case 'L':
					m.d = dir::L;
					break;
				case 'R':
					m.d = dir::R;
					break;
				default:
					throw std::runtime_error("bad direction");
				}
				is >> m.dist;
			}
			return is;
		}
	};
	struct puzzle_input
	{
		std::vector<movement> lines;

		friend std::istream& operator>>(std::istream& is, puzzle_input& input)
		{
			input.lines = std::ranges::istream_view<line<movement>>(is) | std::ranges::to<std::vector<movement>>();

			return is;
		}
	};

	std::int64_t part1(const puzzle_input& input)
	{
		std::int64_t count = 0;
		int position = 50;
		for (const movement& m : input.lines)
		{
			switch (m.d)
			{
			case dir::L:
				position -= m.dist;
				break;
			case dir::R:
				position += m.dist;
				break;
			}
			position %= 100;
			if (position < 0) position += 100;
			if (position == 0) ++count;
		}
		return count;
	}

	std::int64_t part2(const puzzle_input& input)
	{
		std::int64_t count = 0;
		int position = 50;
		for (const movement& m : input.lines)
		{
			int oldposition = position;
			switch (m.d)
			{
			case dir::L:
				position -= m.dist;
				count += div_roundpos(oldposition, 100) - div_roundpos(position, 100);
				break;
			case dir::R:
				position += m.dist;
				count += div_roundneg(position, 100) - div_roundneg(oldposition, 100);
				break;
			}
			position %= 100;
			if (position < 0) position += 100;
		}
		return count;
	}
}
