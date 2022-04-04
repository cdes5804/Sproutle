#include <cstring>
#include <random>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "common/constant.hh"
#include "server/message.hh"
#include "user_db/user_db.hh"

std::string GenRandomString(const uint32_t length) {
  static const std::vector<char> alphanum = {
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
      'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
      'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  std::string tmp_s;
  tmp_s.reserve(length);

  for (uint32_t i = 0; i < length; i++) {
    tmp_s += alphanum[rand() % alphanum.size()];
  }

  return tmp_s;
}

TEST_CASE("Test Create User") {
  UserDB user_db;
  std::unordered_set<UserId> users;
  for (uint32_t i = 0; i < MAX_USER_NUM; i++) {
    auto user_id = user_db.CreateNewUser(GenRandomString(5).c_str());
    REQUIRE(users.find(user_id) == users.end());
    users.insert(user_id);
  }
}

TEST_CASE("Test Get User Answer") {
  UserDB user_db;
  UserId min_user_id;
  UserId max_user_id;
  std::vector<UserId> user_ids(MAX_USER_NUM);
  std::vector<char[WORD_LENGTH + 1]> user_answer(MAX_USER_NUM);
  for (uint32_t i = 0; i < MAX_USER_NUM; i++) {
    strcpy(user_answer[i], GenRandomString(5).c_str());
    auto user_id = user_db.CreateNewUser(user_answer[i]);
    user_ids[i] = user_id;
    min_user_id = i == 0 ? user_id : std::min(min_user_id, user_id);
    max_user_id = i == 0 ? user_id : std::max(max_user_id, user_id);
  }

  user_ids.resize(std::unique(std::begin(user_ids), std::end(user_ids)) -
                  std::begin(user_ids));
  REQUIRE(user_ids.size() == MAX_USER_NUM);

  char answer[WORD_LENGTH + 1];
  for (uint32_t i = 0; i < MAX_USER_NUM; i++) {
    UserId user_id = user_ids[i];
    user_db.GetUserAnswer(user_id, answer);
    REQUIRE(strcmp(answer, user_answer[i]) == 0);
  }

  user_db.GetUserAnswer(min_user_id - 1, answer);
  REQUIRE(strlen(answer) == 0);
  user_db.GetUserAnswer(max_user_id + 1, answer);
  REQUIRE(strlen(answer) == 0);
}

TEST_CASE("Test Set/Get User Guess Count") {
  UserDB user_db;
  std::unordered_map<UserId, uint32_t> user_guess_count;
  UserId min_user_id;
  UserId max_user_id;
  for (uint32_t i = 0; i < MAX_USER_NUM; i++) {
    auto user_id = user_db.CreateNewUser(GenRandomString(5).c_str());
    min_user_id = i == 0 ? user_id : std::min(min_user_id, user_id);
    max_user_id = i == 0 ? user_id : std::max(max_user_id, user_id);
    REQUIRE(user_guess_count.find(user_id) == user_guess_count.end());
    user_guess_count[user_id] = 0;
  }

  for (auto [user_id, guess_count] : user_guess_count) {
    REQUIRE(user_db.GetUserGuessCount(user_id) == guess_count);
    int32_t new_guess_count = std::rand() % 6;
    user_db.SetUserGuessCount(user_id, new_guess_count);
    user_guess_count[user_id] = new_guess_count;
  }

  for (auto [user_id, guess_count] : user_guess_count) {
    REQUIRE(user_db.GetUserGuessCount(user_id) == guess_count);
  }

  REQUIRE(user_db.GetUserGuessCount(min_user_id - 1) == -1);
  REQUIRE(user_db.GetUserGuessCount(max_user_id + 1) == -1);
}

TEST_CASE("Test Remove User") {
  UserDB user_db;
  std::unordered_set<UserId> users;
  UserId min_user_id;
  UserId max_user_id;
  for (uint32_t i = 0; i < MAX_USER_NUM; i++) {
    auto user_id = user_db.CreateNewUser(GenRandomString(5).c_str());
    min_user_id = i == 0 ? user_id : std::min(min_user_id, user_id);
    max_user_id = i == 0 ? user_id : std::max(max_user_id, user_id);
    REQUIRE(users.find(user_id) == users.end());
    users.insert(user_id);
  }

  std::vector<UserId> removed_users;
  for (uint32_t i = 0; i < MAX_USER_NUM && !users.empty(); i += 2) {
    auto user_id_iter = users.begin();
    REQUIRE(user_db.RemoveUserFromDB(*user_id_iter) == true);
    removed_users.emplace_back(*user_id_iter);
    users.erase(user_id_iter);
  }

  REQUIRE(user_db.RemoveUserFromDB(min_user_id - 1) == false);
  REQUIRE(user_db.RemoveUserFromDB(max_user_id + 1) == false);

  for (auto user_id : users) {
    REQUIRE(user_db.GetUserGuessCount(user_id) == 0);
  }

  for (auto removed_user_id : removed_users) {
    REQUIRE(user_db.GetUserGuessCount(removed_user_id) == -1);
  }
}
