#include <iostream>
#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "argument_parser/argument_parser.hh"

// NOLINTNEXTLINE
TEST_CASE("Test Argument Parser") {
  // clang-format off
  std::vector<std::string> args = {"program_name",
                                   "--arg1", "../path/to/random/file",
                                   "--arg2", "123",
                                   "--arg3", "-123",
                                   "--arg4", "True",
                                   "--arg5", "false",
                                   "--arg6", "3.14159",
                                   "--arg7", "-3.14159"};
  // clang-format on
  int argc = static_cast<int>(args.size());
  char **argv = new char *[argc];
  for (size_t i = 0; i < argc; i++) {
    argv[i] = new char[args[i].length() + 1];
    strncpy(argv[i], args[i].c_str(), args[i].length() + 1);
  }

  ArgumentParser parser(argc, argv);

  SECTION("Test Parse String") {
    REQUIRE(parser.ParseArgument<std::string>("arg1") ==
            "../path/to/random/file");
    REQUIRE(parser.ParseArgument<std::string>("argX", "not found") ==
            "not found");
    REQUIRE(parser.ParseArgument<std::string>("argX").empty());
  }

  SECTION("Test Parse Int") {
    REQUIRE(parser.ParseArgument<int32_t>("arg2") == 123);
    REQUIRE(parser.ParseArgument<int32_t>("arg3") == -123);
    REQUIRE(parser.ParseArgument<int32_t>("argX", 456) == 456);
    REQUIRE(parser.ParseArgument<int32_t>("argX") == 0);
  }

  SECTION("Test Parse Bool") {
    REQUIRE(parser.ParseArgument<bool>("arg4") == true);
    REQUIRE(parser.ParseArgument<bool>("arg5") == false);
    REQUIRE(parser.ParseArgument<bool>("argX", true) == true);
    REQUIRE(parser.ParseArgument<bool>("argX") == false);
  }

  SECTION("Test Parse Float") {
    REQUIRE(parser.ParseArgument<double>("arg6") == 3.14159);
    REQUIRE(parser.ParseArgument<double>("arg7") == -3.14159);
    REQUIRE(parser.ParseArgument<double>("argX", 0.123) == 0.123);
    REQUIRE(parser.ParseArgument<double>("argX") == 0.0);
  }
}