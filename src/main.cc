#include <iostream>
#include <memory>
#include <string>

#include "argument_parser/argument_parser.hh"
#include "dictionary/dictionary.hh"
#include "server/server.hh"
#include "user_db/user_db.hh"

int main(int argc, char* argv[]) {
  ArgumentParser parser(argc, argv);
  std::string answer_pool_path = parser.ParseArgument<std::string>("answer_pool", "../word_list/answer.json");
  std::string allowed_guesses_path = parser.ParseArgument<std::string>("allowed_guess", "../word_list/guess.json");
  uint16_t port = parser.ParseArgument<uint16_t>("port", 5000);
  uint32_t thread_num = parser.ParseArgument<uint32_t>("thread_num", 4);

  auto dictionary = std::make_unique<Dictionary>(answer_pool_path, allowed_guesses_path);
  auto user_db = std::make_unique<UserDB>();

  auto server = WordleServer(std::move(dictionary), std::move(user_db));
  server.Run(port, thread_num);
  return 0;
}