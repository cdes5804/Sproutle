#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP

#include <mutex>
#include <random>
#include <string>
#include <vector>

#include "common/constant.hh"

const uint32_t MAX_WORD_LIST_LENGTH = 12000;

class Dictionary {
private:
  char last_returned_word_[WORD_LENGTH + 1] = {};
  uint32_t answer_pool_size_ = 0;
  uint32_t allowed_guesses_size_ = 0;
  const uint32_t maximum_common_characters_allowed_;
  char answer_pool_[MAX_WORD_LIST_LENGTH][WORD_LENGTH + 1];
  char allowed_guesses_[MAX_WORD_LIST_LENGTH][WORD_LENGTH + 1];
  std::mutex mu_;
  uint32_t GetCommonCharacterCount(char a[], char b[]) const;

public:
  explicit Dictionary(const std::string &answer_pool_file_path,
                      const std::string &allowed_guesses_file_path,
                      uint32_t maximum_common_characters_allowed = 2);

  /**
   * Get a new word from `answer_pool_` for the Wordle game.
   * This function makes sure no consecutive words returned by it are the same
   * or share more than `maximum_common_characters_allowed_` characters if the
   * answer pool allows.
   * @param word The buffer to put the string that should be returned
   */
  void GetNewWord(char word[]);

  /**
   * Query whether a given word is a valid guess.
   * @param word The word to check
   * @return true if it is valid, i.e. the word exists in `answer_pool_` or
   * `allowed_guesses_`, false otherwise
   */
  bool IsWordInDictionary(const char word[]) const;
};

#endif