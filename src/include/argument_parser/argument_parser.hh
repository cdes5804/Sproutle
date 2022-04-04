#ifndef ARG_PARSER_HH
#define ARG_PARSER_HH

#include <algorithm>
#include <map>
#include <string>
#include <vector>

class ArgumentParser {
private:
  uint32_t argc_;
  std::vector<std::string> argv_;

public:
  ArgumentParser(int argc, char *argv[]);

  /**
   * Process the command line argument to find the value for a given flag and
   * return it as the given type.
   * @param flag The flag for which we want to find the value for
   * @param default_value [optional] The value to return if the flag is not
   * specified by user
   * @return The value found in command line argument cast to the given type or
   * `default_value`
   */
  template <typename T>
  T ParseArgument(const std::string &flag, const T &default_value = T()) {
    const std::string full_flag = "--" + flag;
    for (uint32_t i = 1; i < argc_ - 1; i++) {
      if (full_flag == argv_[i]) {
        return static_cast<T>(std::stod(argv_[i + 1]));
      }
    }
    return default_value;
  }
};

/** std::string specification for ParseArgument. */
template <>
std::string ArgumentParser::ParseArgument(const std::string &flag,
                                          const std::string &default_value);

/** bool specification for ParseArgument. */
template <>
bool ArgumentParser::ParseArgument(const std::string &flag,
                                   const bool &default_value);

#endif