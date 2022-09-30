#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
//#include "catch.hpp"

#include "recognizer.h"

TEST_CASE( "isStopword", "[repository]" ) {

   constexpr auto txt1{"What is the weather like today?"};
   constexpr auto txt2{"What is the weather like in Paris today ?"};
   constexpr auto txt3{"Tell me an interesting fact."};

	Repository repo;

	CHECK(repo.isStopword("is"));
	CHECK(repo.isStopword("in"));
	CHECK(repo.isStopword("an"));

	BENCHMARK("IsStopword 1") {
		return repo.isStopword("is");
	};
	
	BENCHMARK("IsStopword 2") {
		return repo.isStopword("in");
	};

	BENCHMARK("IsStopword 3") {
		return repo.isStopword("an");
	};
}

TEST_CASE( "normalize a string", "[recognizer]" ) {

	constexpr auto txt1{"The dog jumps high! 123"};
	constexpr auto txt2{"nobody  is   perfect :/"};
	constexpr auto txt3{"Once uppon a Time... "};

	Recognizer engine;

	CHECK_THAT(engine.normalize(txt1), 
		Catch::Equals(StringList{"the", "dog", "jumps", "high"}));

	CHECK_THAT(engine.normalize(txt2), 
		Catch::Equals(StringList{"nobody", "is", "perfect"}));

	CHECK_THAT(engine.normalize(txt3), 
		Catch::Equals(StringList{"once", "uppon", "a", "time"}));

	BENCHMARK("Normalize 1") {
		return engine.normalize(txt1);
	};
	
	BENCHMARK("Normalize 2") {
		return engine.normalize(txt2);
	};

	BENCHMARK("Normalize 3") {
		return engine.normalize(txt3);
	};
}

TEST_CASE( "extract words without stopwords", "[recognizer]" ) {

	StringList arr1{"what", "is", "the", "weather", "like", "today"};
	StringList arr2{"what", "is", "the", "weather", "like", "in", "paris", "today"};
	StringList arr3{"tell", "me", "an", "interesting", "fact"};

	Recognizer engine;

	CHECK_THAT(engine.exctract(arr1), 
		Catch::Equals(ResultList{{Category::Get, Category::Weather}}));

	CHECK_THAT(engine.exctract(arr2), 
		Catch::Equals(ResultList{{Category::Get, Category::Weather, Category::City}}));

	CHECK_THAT(engine.exctract(arr3), 
		Catch::Equals(ResultList{{Category::Fact}}));

	BENCHMARK("Exctract 1") {
		return engine.exctract(arr1);
	};
	
	BENCHMARK("Exctract 2") {
		return engine.exctract(arr2);
	};

	BENCHMARK("Exctract 3") {
		return engine.exctract(arr3);
	};
}

TEST_CASE( "calculate intention", "[recognizer]" ) {

   constexpr auto txt1{"What is the weather like today?"};
   constexpr auto txt2{"What is the weather like in Paris today ?"};
   constexpr auto txt3{"Tell me an interesting fact."};

	Recognizer engine;

	CHECK_THAT(engine.calculate(txt1), 
		Catch::Equals(ResultList{{Category::Get, Category::Weather}}));

	CHECK_THAT(engine.calculate(txt2), 
		Catch::Equals(ResultList{{Category::Get, Category::Weather, Category::City}}));

	CHECK_THAT(engine.calculate(txt3), 
		Catch::Equals(ResultList{{Category::Fact}}));

	BENCHMARK("Calculate 1") {
		return engine.calculate(txt1);
	};
	
	BENCHMARK("Calculate 2") {
		return engine.calculate(txt2);
	};

	BENCHMARK("Calculate 3") {
		return engine.calculate(txt3);
	};
}
