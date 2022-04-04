#include <cstring>
#include <fstream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "common/constant.hh"
#include "dictionary/dictionary.hh"
#include "server/server.hh"
#include "user_db/user_db.hh"
#include "json/json.hpp"

using json = nlohmann::json;

void WriteTempJsonFile(const std::string &filename, const json &j) {
  std::ofstream o(filename);
  o << std::setw(4) << j << std::endl;
}

void CleanTempFile(const std::string &filename) {
  std::remove(filename.c_str());
}

TEST_CASE("Test Connect") {
  const std::string answer_pool_path = "../../word_list/answer.json";
  const std::string allowed_guess_path = "../../word_list/guess.json";
  auto dictionary =
      std::make_unique<Dictionary>(answer_pool_path, allowed_guess_path);
  auto user_db = std::make_unique<UserDB>();
  auto server = WordleServer(std::move(dictionary), std::move(user_db));

  std::unordered_set<UserId> user_ids;
  for (uint32_t i = 0; i < MAX_USER_NUM; i++) {
    UserId id = server.Connect();
    REQUIRE(user_ids.find(id) == user_ids.end());
    user_ids.insert(id);
  }
}

TEST_CASE("Test Invalid User") {
  json j = json::array({"group"});
  json j2 = json::array({});
  const std::string answer_pool_path = "TempAnswerPool.json";
  const std::string allowed_guess_path = "TempAllowedGuess.json";
  WriteTempJsonFile(answer_pool_path, j);
  WriteTempJsonFile(allowed_guess_path, j2);
  auto dictionary =
      std::make_unique<Dictionary>(answer_pool_path, allowed_guess_path);
  auto user_db = std::make_unique<UserDB>();
  auto server = WordleServer(std::move(dictionary), std::move(user_db));

  GuessArgs args;
  GuessReply reply;
  const uint32_t test_round = 5'000U;
  for (uint32_t i = 0; i < test_round; i++) {
    args.user_id_ = std::rand() % (100 * test_round);
    strncpy(args.guess_, "apple", WORD_LENGTH + 1);
    server.Guess(args, &reply);
    REQUIRE(reply.reply_status_ == ReplyStatus::INVALID_USER);
  }

  CleanTempFile(answer_pool_path);
  CleanTempFile(allowed_guess_path);
}

TEST_CASE("Test Guess Invalid Word") {
  json j = json::array({"group"});
  json j2 = json::array({});
  const std::string answer_pool_path = "TempAnswerPool.json";
  const std::string allowed_guess_path = "TempAllowedGuess.json";
  WriteTempJsonFile(answer_pool_path, j);
  WriteTempJsonFile(allowed_guess_path, j2);
  auto dictionary =
      std::make_unique<Dictionary>(answer_pool_path, allowed_guess_path);
  auto user_db = std::make_unique<UserDB>();
  auto server = WordleServer(std::move(dictionary), std::move(user_db));

  UserId user_id = server.Connect();
  GuessArgs args;
  GuessReply reply;

  args.user_id_ = user_id;
  strncpy(args.guess_, "apple", WORD_LENGTH + 1);
  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::WORD_NOT_FOUND);

  strncpy(args.guess_, "aaaaa", WORD_LENGTH + 1);
  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::WORD_NOT_FOUND);

  strncpy(args.guess_, "bb", WORD_LENGTH + 1);
  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::WORD_NOT_FOUND);

  strncpy(args.guess_, "group", WORD_LENGTH + 1);
  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::WON);

  for (const GuessResult guess_result : reply.guess_results_) {
    REQUIRE(guess_result == GuessResult::CORRECT);
  }

  /* the user should be deleted after finishing the game */
  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::INVALID_USER);

  CleanTempFile(answer_pool_path);
  CleanTempFile(allowed_guess_path);
}

TEST_CASE("Test Guess Limit") {
  json j = json::array({"group"});
  json j2 = json::array({"speed"});
  const std::string answer_pool_path = "TempAnswerPool.json";
  const std::string allowed_guess_path = "TempAllowedGuess.json";
  WriteTempJsonFile(answer_pool_path, j);
  WriteTempJsonFile(allowed_guess_path, j2);
  auto dictionary =
      std::make_unique<Dictionary>(answer_pool_path, allowed_guess_path);
  auto user_db = std::make_unique<UserDB>();
  auto server = WordleServer(std::move(dictionary), std::move(user_db));
  const uint32_t guess_limit = 6;

  UserId user_id = server.Connect();
  GuessArgs args;
  GuessReply reply;

  args.user_id_ = user_id;
  strncpy(args.guess_, "speed", WORD_LENGTH + 1);

  for (uint32_t i = 0; i < guess_limit - 1; i++) {
    server.Guess(args, &reply);
    REQUIRE(reply.reply_status_ == ReplyStatus::OK);
  }

  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::LOST);

  /* after a user has reached its limit, it should be deleted */
  server.Guess(args, &reply);
  REQUIRE(reply.reply_status_ == ReplyStatus::INVALID_USER);

  CleanTempFile(answer_pool_path);
  CleanTempFile(allowed_guess_path);
}
