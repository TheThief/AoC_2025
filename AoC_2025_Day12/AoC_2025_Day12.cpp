#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "AoC_2025_Day12.h"

using namespace aoc2025::day12;

int main()
{
	SetConsoleOutputCP(CP_UTF8);

	std::ifstream i("input.txt");
	puzzle_input input;
	i >> input;

	{
		const auto start = std::chrono::high_resolution_clock::now();
		const auto part1_result = part1(input);
		const auto stop = std::chrono::high_resolution_clock::now();
		std::cout << "Part 1: " << part1_result << " in " << std::chrono::duration<float, std::micro>(stop - start).count() << "μs" << std::endl;
	}

	{
		const auto start = std::chrono::high_resolution_clock::now();
		const auto part2_result = part2(input);
		const auto stop = std::chrono::high_resolution_clock::now();
		std::cout << "Part 2: " << part2_result << " in " << std::chrono::duration<float, std::micro>(stop - start).count() << "μs" << std::endl;
	}
}







