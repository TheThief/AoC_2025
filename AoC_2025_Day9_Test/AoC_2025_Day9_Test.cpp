#include "CppUnitTest.h"

#include "AoC_2025_Day9/AoC_2025_Day9.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace aoc2025::day9::test
{
	const std::string_view test_input_1 =
R"(7,1
11,1
11,7
9,7
9,5
2,5
2,3
7,3)"sv;

	const std::string_view test_input_2 = test_input_1;

	const int64_t expected_test_ouput_1 = 50;
	const int64_t expected_test_ouput_2 = 24;

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




