#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
//#include "catch.hpp"

#include "recognizer.h"

static constexpr auto txt1{"The dog jumps high! 123"};
static constexpr auto txt2{"nobody  is   perfect :/"};
static constexpr auto txt3{"Once uppon a Time... "};


TEST_CASE( "normalize a string", "[recognizer]" ) {

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

