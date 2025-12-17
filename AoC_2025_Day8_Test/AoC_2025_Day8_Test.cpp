#include "CppUnitTest.h"

#include "AoC_2025_Day8/AoC_2025_Day8.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace aoc2025::day8::test
{
	const std::string_view test_input_1 =
R"(162,817,812
57,618,57
906,360,560
592,479,940
352,342,300
466,668,158
542,29,236
431,825,988
739,650,466
52,470,668
216,146,977
819,987,18
117,168,530
805,96,715
346,949,466
970,615,88
941,993,340
862,61,35
984,92,344
425,690,689)"sv;

	const std::string_view test_input_2 = test_input_1;

	const int64_t expected_test_ouput_1 = 40;
	const int64_t expected_test_ouput_2 = 25272;

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



