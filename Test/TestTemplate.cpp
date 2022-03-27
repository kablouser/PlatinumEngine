#include <catch2/catch.hpp>

// To save you some time reading the docs, either look at an actual example
// 	or read this quick summary
// Use TEST_CASE("name", "[tag1][tagn]") to declare a test where tags are used to
// 	run a sub group of all tests (more details below)
// Use REQUIRE(expression) as an assertion, e.g. REQUIRE(x==2). If this is
// 	false, execution will stop immediately and no further tests will run
// Use CHECK(expression) as a test, e.g. CHECK(x==2). If this is false,
// 	execution will continue and remaining tests will run but will print out
// 	an error at the end
// Read the docs for more info
// https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md

// Running the tests:
// Run all the tests by running the tests configuration instead of PlatinumEngine
// If you want to run tests with specific tags (the second arg in TEST_CASE)
// 	then go to edit configurations and edit the tests configuration.
// 	Add the tags into the "Program arguments" box like [tag1][tag2]
// 	Click apply then ok. Don't forget to remove them again when you
// 	want to run different unit tests

unsigned int Factorial(unsigned int number)
{
	return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials", "[template]")
{
	REQUIRE(Factorial(1) == 1);
	REQUIRE(Factorial(2) == 2);
	REQUIRE(Factorial(3) == 6);
	REQUIRE(Factorial(10) == 3628800);
}