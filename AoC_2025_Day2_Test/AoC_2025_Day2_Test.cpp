#include "CppUnitTest.h"

#include "AoC_2025_Day2/AoC_2025_Day2.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace aoc2025::day2::test
{
	const std::string_view test_input_1 =
R"(11-22,95-115,998-1012,1188511880-1188511890,222220-222224,
1698522-1698528,446443-446449,38593856-38593862,565653-565659,
824824821-824824827,2121212118-2121212124)"sv;

	const std::string_view test_input_2 = test_input_1;

	const int64_t expected_test_ouput_1 = 1227775554;
	const int64_t expected_test_ouput_2 = 4174379265;

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

