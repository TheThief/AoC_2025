#include "CppUnitTest.h"

#include "AoC_2025_Day3/AoC_2025_Day3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace aoc2025::day3::test
{
	const std::string_view test_input_1 =
R"(987654321111111
811111111111119
234234234234278
818181911112111)"sv;

	const std::string_view test_input_2 = test_input_1;

	const int64_t expected_test_ouput_1 = 357;
	const int64_t expected_test_ouput_2 = 3121910778619;

	TEST_CLASS(Test)
	{
	public:
		Test()
		{
		}

		TEST_METHOD(Part1)
		{
			puzzle_input input;
			std::ispanstream(test_input_1) >> input;
			Assert::AreEqual(expected_test_ouput_1, part1(input));
		}

		TEST_METHOD(Part2)
		{
			puzzle_input input;
			std::ispanstream(test_input_2) >> input;
			Assert::AreEqual(expected_test_ouput_2, part2(input));
		}
	};
}

