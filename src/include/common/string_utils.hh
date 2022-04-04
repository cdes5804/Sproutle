#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

/**
 * Remove whitespaces in the beginning of a string.
 * @param s The string used for trimming
 * @return A new string with whitespaces in the beginning of `s` removed
 */
std::string LeftTrim(const std::string& s);

/**
 * Remove whitespaces in the end of a string.
 * @param s The string used for trimming
 * @return A new string with whitespaces in the end of `s` removed
 */
std::string RightTrim(const std::string& s);

/**
 * Remove whitespaces from both ends of a string.
 * @param s The string used for trimming
 * @return A new string with whitespaces in both ends of `s` removed
 */
std::string Trim(const std::string& s);

/**
 * Transform each character in a string to lowercase.
 * @param s The string used for transformation
 * @return A new string with each character in `s` made lowercase
 */
std::string ToLower(const std::string& s);

/**
 * Check if a character is present in the given string.
 * @param c The character to find
 * @param s The string to find `c` in
 * @return true if `c` is present in `s`, false otherwise
 */
bool IsCharacterInString(char c, const std::string& s);

#endif