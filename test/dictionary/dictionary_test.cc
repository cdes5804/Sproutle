#include <algorithm>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include "common/constant.hh"
#include "dictionary/dictionary.hh"
#include "json/json.hpp"

using json = nlohmann::json;

const uint32_t MAX_COMMON_CHAR_ALLOWED = 2U;

void WriteTempJsonFile(const std::string& filename, const json& j) {
  std::ofstream o(filename);
  o << std::setw(4) << j << std::endl;
}

void CleanTempFile(const std::string& filename) { std::remove(filename.c_str()); }

uint32_t GetCommonCharacterCount(const std::string& a, const std::string& b) {
  if (a.empty() || b.empty()) {
    return 0U;
  }

  assert(a.length() == b.length() && "GetCommonCharacterCount: Length of strings differ");
  std::string s = a;
  std::string t = b;
  std::sort(std::begin(s), std::end(s));
  std::sort(std::begin(t), std::end(t));

  uint32_t s_count = 0;
  uint32_t t_count = 0;
  uint32_t common_count = 0;
  while (s_count < s.length() && t_count < t.length()) {
    if (s[s_count] < t[t_count]) {
      s_count++;
    } else if (s[s_count] > t[t_count]) {
      t_count++;
    } else {
      s_count++;
      t_count++;
      common_count++;
    }
  }
  return common_count;
}

TEST_CASE("Test New Word Satisfies Rule 1") {
  const std::string answer_pool_path = "../../word_list/answer.json";
  const std::string allowed_guess_path = "../../word_list/guess.json";
  Dictionary dictionary(answer_pool_path, allowed_guess_path, MAX_COMMON_CHAR_ALLOWED);

  const uint32_t test_round = MAX_USER_NUM;
  std::string last_word;
  char word[WORD_LENGTH + 1];
  for (uint32_t i = 0; i < test_round; i++) {
    dictionary.GetNewWord(word);
    REQUIRE(GetCommonCharacterCount(last_word, word) <= MAX_COMMON_CHAR_ALLOWED);
    last_word = std::string(word);
  }
}

TEST_CASE("Test New Word Satisfies Rule 2") {
  json j = json::array({"aaaaa", "aaabb", "bbaaa"});
  const std::string answer_pool_path = "TempAnswerPool.json";
  const std::string allowed_guess_path = "../../word_list/guess.json";
  WriteTempJsonFile(answer_pool_path, j);
  Dictionary dictionary(answer_pool_path, allowed_guess_path, MAX_COMMON_CHAR_ALLOWED);

  const uint32_t test_round = 10U;
  std::string last_word;
  char word[WORD_LENGTH + 1];
  for (uint32_t i = 0; i < test_round; i++) {
    dictionary.GetNewWord(word);
    REQUIRE(last_word != std::string(word));
    last_word = std::string(word);
  }

  CleanTempFile(answer_pool_path);
}

TEST_CASE("Test Only One Valid Word") {
  json j = json::array({"sport"});
  const std::string temp_file_path = "TempFile.json";
  WriteTempJsonFile(temp_file_path, j);
  Dictionary dictionary(temp_file_path, temp_file_path, MAX_COMMON_CHAR_ALLOWED);

  REQUIRE(dictionary.IsWordInDictionary("sport") == true);
  REQUIRE(dictionary.IsWordInDictionary("sugar") == false);
  REQUIRE(dictionary.IsWordInDictionary("giant") == false);
  REQUIRE(dictionary.IsWordInDictionary("train") == false);

  char word[WORD_LENGTH + 1] = {};
  dictionary.GetNewWord(word);
  REQUIRE(std::string(word) == "sport");
  memset(word, 0, WORD_LENGTH + 1);
  dictionary.GetNewWord(word);
  REQUIRE(std::string(word) == "sport");  // should still return sport

  CleanTempFile(temp_file_path);
}

TEST_CASE("Test Valid And Invaid Word") {
  const std::string answer_pool_path = "../../word_list/answer.json";
  const std::string allowed_guess_path = "../../word_list/guess.json";
  Dictionary dictionary(answer_pool_path, allowed_guess_path, MAX_COMMON_CHAR_ALLOWED);

  std::vector<std::string> word_pool;
  std::ifstream answer_pool_file(answer_pool_path);
  std::ifstream allowed_guesses_file(allowed_guess_path);

  json j;
  answer_pool_file >> j;
  for (const auto& word : j) {
    word_pool.emplace_back(static_cast<std::string>(word));
  }

  json j2;
  allowed_guesses_file >> j2;
  for (const auto& word : j2) {
    word_pool.emplace_back(static_cast<std::string>(word));
  }

  for (const auto& word : word_pool) {
    REQUIRE(dictionary.IsWordInDictionary(word.c_str()) == true);
  }

  REQUIRE(dictionary.IsWordInDictionary("") == false);
  REQUIRE(dictionary.IsWordInDictionary("abcde") == false);
  REQUIRE(dictionary.IsWordInDictionary("longer") == false);
}
