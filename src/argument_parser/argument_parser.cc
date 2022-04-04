#include "argument_parser/argument_parser.hh"
#include "common/string_utils.hh"

ArgumentParser::ArgumentParser(int argc, char* argv[]) {
  this->argc_ = argc;
  for (uint32_t i = 0; i < this->argc_; i++) {
    this->argv_.emplace_back(Trim(argv[i]));
  }
}

template <>
std::string ArgumentParser::ParseArgument(const std::string& flag, const std::string& default_value) {
  const std::string full_flag = "--" + flag;
  for (uint32_t i = 1; i < argc_ - 1; i++) {
    if (full_flag == argv_[i]) {
      return argv_[i + 1];
    }
  }
  return default_value;
}

template <>
bool ArgumentParser::ParseArgument(const std::string& flag, const bool& default_value) {
  const std::string full_flag = "--" + flag;
  for (uint32_t i = 1; i < argc_ - 1; i++) {
    if (full_flag == argv_[i]) {
      std::string value = argv_[i + 1];

      if (std::all_of(value.begin(), value.end(), [](unsigned char c) { return std::isdigit(c); })) {
        return std::stoll(value) != 0;
      }

      std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
      return value == "true";
    }
  }
  return default_value;
}