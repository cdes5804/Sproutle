#include "common/string_utils.hh"

#include <algorithm>

std::string LeftTrim(const std::string &s) {
  std::string t = s;
  t.erase(t.begin(), std::find_if(t.begin(), t.end(), [](unsigned char ch) {
            return std::isspace(ch) == 0;
          }));
  return t;
}

std::string RightTrim(const std::string &s) {
  std::string t = s;
  t.erase(std::find_if(t.rbegin(), t.rend(),
                       [](unsigned char ch) { return std::isspace(ch) == 0; })
              .base(),
          t.end());
  return t;
}

std::string Trim(const std::string &s) { return LeftTrim(RightTrim(s)); }

std::string ToLower(const std::string &s) {
  std::string t = s;
  for (char &c : t) {
    c = static_cast<char>(std::tolower(c));
  }
  return t;
}

bool IsCharacterInString(char c, const std::string &s) {
  return s.find(c) != std::string::npos;
}