#include <algorithm>
#include <cstring>
#include <fstream>
#include <string>

#include "common/constant.hh"
#include "dictionary/dictionary.hh"
#include "json/json.hpp"

using json = nlohmann::json;

uint32_t Dictionary::GetCommonCharacterCount(char a[], char b[]) const {
  if (a == nullptr || b == nullptr) {
    return 0;
  }
  uint32_t a_length = strlen(a);
  uint32_t b_length = strlen(b);

  if (a_length == 0 || b_length == 0) {
    return 0;
  }

  assert(a_length == WORD_LENGTH);
  assert(b_length == WORD_LENGTH);
  assert(a_length == b_length &&
         "GetCommonCharacterCount: Length of strings differ");
  char s[WORD_LENGTH + 1];
  char t[WORD_LENGTH + 1];

  strncpy(s, a, WORD_LENGTH + 1);
  strncpy(t, b, WORD_LENGTH + 1);

  std::sort(s, s + a_length);
  std::sort(t, t + b_length);

  uint32_t s_count = 0;
  uint32_t t_count = 0;
  uint32_t common_count = 0;
  while (s_count < a_length && t_count < b_length) {
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

Dictionary::Dictionary(const std::string &answer_pool_file_path,
                       const std::string &allowed_guesses_file_path,
                       uint32_t maximum_common_characters_allowed)
    : maximum_common_characters_allowed_(maximum_common_characters_allowed) {
  // read word list from json file
  std::ifstream answer_pool_file(answer_pool_file_path);
  std::ifstream allowed_guesses_file(allowed_guesses_file_path);

  assert(answer_pool_file.good() && "answer pool cannot be read");
  assert(allowed_guesses_file.good() && "allowed guesses cannot be read");

  json j;
  answer_pool_file >> j;
  answer_pool_size_ = 0;
  for (const auto &word : j) {
    strncpy(answer_pool_[answer_pool_size_],
            static_cast<std::string>(word).c_str(), WORD_LENGTH + 1);
    answer_pool_size_++;
  }

  json j2;
  allowed_guesses_file >> j2;
  allowed_guesses_size_ = 0;
  for (const auto &word : j2) {
    strncpy(allowed_guesses_[allowed_guesses_size_],
            static_cast<std::string>(word).c_str(), WORD_LENGTH + 1);
    allowed_guesses_size_++;
  }
}

void Dictionary::GetNewWord(char word[]) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Get a new word from `answer_pool_` that satisfies the rules mentioned in
   * `include/dictionary/dictionary.hh`. You should put the string in `word`.
   */
}

bool Dictionary::IsWordInDictionary(const char word[]) const {
  /**
   * TODO:
   * Check if a given word is in the dictionary.
   * You have to check both `answer_pool_` and `allowed_guessess_`.
   * If the word appears in any of them then you should return true.
   */
  return true;
}
