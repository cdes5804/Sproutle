#include <string>

#include <catch2/catch_test_macros.hpp>

#include "common/string_utils.hh"

// NOLINTNEXTLINE
TEST_CASE("Test String Utils") {
  const std::string left_spaces = "  Abc";
  const std::string right_spaces = "aBc  ";
  const std::string two_ends_spaces = "   ABC ";
  const std::string no_space = "abc";
  const std::string space_in_middle = " a b c ";

  SECTION("Test LeftTrim") {
    std::string trimmed_left_spaces = LeftTrim(left_spaces);
    REQUIRE(trimmed_left_spaces == "Abc");

    std::string trimmed_right_spaces = LeftTrim(right_spaces);
    REQUIRE(trimmed_right_spaces == right_spaces);

    std::string trimmed_two_ends_spaces = LeftTrim(two_ends_spaces);
    REQUIRE(trimmed_two_ends_spaces == "ABC ");

    std::string trimmed_no_space = LeftTrim(no_space);
    REQUIRE(trimmed_no_space == no_space);

    std::string trimmed_space_in_middle = LeftTrim(space_in_middle);
    REQUIRE(trimmed_space_in_middle == "a b c ");
  }

  SECTION("Test RightTrim") {
    std::string trimmed_left_spaces = RightTrim(left_spaces);
    REQUIRE(trimmed_left_spaces == left_spaces);

    std::string trimmed_right_spaces = RightTrim(right_spaces);
    REQUIRE(trimmed_right_spaces == "aBc");

    std::string trimmed_two_ends_spaces = RightTrim(two_ends_spaces);
    REQUIRE(trimmed_two_ends_spaces == "   ABC");

    std::string trimmed_no_space = RightTrim(no_space);
    REQUIRE(trimmed_no_space == no_space);

    std::string trimmed_space_in_middle = RightTrim(space_in_middle);
    REQUIRE(trimmed_space_in_middle == " a b c");
  }

  SECTION("Test Trim") {
    std::string trimmed_left_spaces = Trim(left_spaces);
    REQUIRE(trimmed_left_spaces == "Abc");

    std::string trimmed_right_spaces = Trim(right_spaces);
    REQUIRE(trimmed_right_spaces == "aBc");

    std::string trimmed_two_ends_spaces = Trim(two_ends_spaces);
    REQUIRE(trimmed_two_ends_spaces == "ABC");

    std::string trimmed_no_space = Trim(no_space);
    REQUIRE(trimmed_no_space == no_space);

    std::string trimmed_space_in_middle = Trim(space_in_middle);
    REQUIRE(trimmed_space_in_middle == "a b c");
  }

  SECTION("Test ToLower") {
    std::string trimmed_left_spaces = ToLower(left_spaces);
    REQUIRE(trimmed_left_spaces == "  abc");

    std::string trimmed_right_spaces = ToLower(right_spaces);
    REQUIRE(trimmed_right_spaces == "abc  ");

    std::string trimmed_two_ends_spaces = ToLower(two_ends_spaces);
    REQUIRE(trimmed_two_ends_spaces == "   abc ");

    std::string trimmed_no_space = ToLower(no_space);
    REQUIRE(trimmed_no_space == no_space);

    std::string trimmed_space_in_middle = ToLower(space_in_middle);
    REQUIRE(trimmed_space_in_middle == space_in_middle);
  }
}
