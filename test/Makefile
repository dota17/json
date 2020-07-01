##########################################################################
# unit tests
##########################################################################

# additional flags
CXXFLAGS += -std=c++11 -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wno-ctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=5 -Wswitch -Wundef -Wno-unused -Wnon-virtual-dtor -Wreorder -Wdeprecated -Wno-float-equal
CPPFLAGS += -I ../single_include -I . -I thirdparty/doctest -I thirdparty/fifo_map -DDOCTEST_CONFIG_SUPER_FAST_ASSERTS

SOURCES = src/unit.cpp \
          src/unit-conversions.cpp \

OBJECTS = $(SOURCES:.cpp=.o)

TESTCASES = $(patsubst src/unit-%.cpp,test-%,$(wildcard src/unit-*.cpp))

##############################################################################
# main rules
##############################################################################

all: $(TESTCASES)

clean:
	rm -fr json_unit $(OBJECTS) $(SOURCES:.cpp=.gcno) $(SOURCES:.cpp=.gcda) $(TESTCASES) $(FUZZERS) test_data.hpp

test_data.hpp:
	@echo "#define TEST_DATA_DIRECTORY" > $@

##############################################################################
# single test file
##############################################################################

json_unit: $(OBJECTS) ../single_include/nlohmann/json.hpp thirdparty/doctest/doctest.h
	@echo "[CXXLD] $@"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) -o $@

%.o: %.cpp ../single_include/nlohmann/json.hpp thirdparty/doctest/doctest.h test_data.hpp
	@echo "[CXX]   $@"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@


##############################################################################
# individual test cases
##############################################################################

test-%: src/unit-%.o src/unit.o ../single_include/nlohmann/json.hpp thirdparty/doctest/doctest.h test_data.hpp
	@echo "[CXXLD] $@"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $< src/unit.o -o $@

TEST_PATTERN ?= "*"
TEST_PREFIX = ""
check: $(OBJECTS) $(TESTCASES)
	@cd .. ; for testcase in $(TESTCASES); do echo "Executing $$testcase..."; $(TEST_PREFIX)test/$$testcase $(TEST_PATTERN) || exit 1; done


##############################################################################
# fuzzer
##############################################################################

FUZZER_ENGINE = src/fuzzer-driver_afl.cpp
FUZZERS = parse_afl_fuzzer parse_bson_fuzzer parse_cbor_fuzzer parse_msgpack_fuzzer parse_ubjson_fuzzer
fuzzers: $(FUZZERS)

parse_afl_fuzzer:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FUZZER_ENGINE) src/fuzzer-parse_json.cpp -o $@

parse_bson_fuzzer:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FUZZER_ENGINE) src/fuzzer-parse_bson.cpp -o $@

parse_cbor_fuzzer:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FUZZER_ENGINE) src/fuzzer-parse_cbor.cpp -o $@

parse_msgpack_fuzzer:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FUZZER_ENGINE) src/fuzzer-parse_msgpack.cpp -o $@

parse_ubjson_fuzzer:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FUZZER_ENGINE) src/fuzzer-parse_ubjson.cpp -o $@
