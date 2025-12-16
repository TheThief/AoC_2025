#include "CppUnitTest.h"

#include "AoC_2025_Day6/AoC_2025_Day6.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace aoc2025::day6::test
{
	const std::string_view test_input_1 =
R"(123 328  51 64 
 45 64  387 23 
  6 98  215 314
*   +   *   +  )"sv;

	const std::string_view test_input_2 = test_input_1;

	const int64_t expected_test_ouput_1 = 4277556;
	const int64_t expected_test_ouput_2 = 3263827;

	TEST_CLASS(Test)
	{
	public:
		Test()
		{
		}

		TEST_METHOD(Part1)
		{
			puzzle_input_1 input;
			std::ispanstream(test_input_1) >> input;
			Assert::AreEqual(expected_test_ouput_1, part1(input));
		}

		TEST_METHOD(Part2)
		{
			puzzle_input_2 input;
			std::ispanstream(test_input_2) >> input;
			Assert::AreEqual(expected_test_ouput_2, part2(input));
		}
	};
}

