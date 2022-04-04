#include <cstring>
#include <iostream>

#include "common/constant.hh"
#include "common/string_utils.hh"
#include "server/server.hh"

WordleServer::WordleServer(std::unique_ptr<Dictionary> dictionary, std::unique_ptr<UserDB> user_db)
    : dictionary_(std::move(dictionary)), user_db_(std::move(user_db)) {}

UserId WordleServer::Connect() {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * A user has connected to the server.
   * Give the user an word to guess (use Dictionary) and put the user into
   * UserDB, and then return the user id for this new user.
   */

  return 0;
}

void WordleServer::Guess(const GuessArgs& args, GuessReply* reply) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Process the information in `args` and fill the appropriate values into
   * `reply`. You can refer to `include/server/server.hh` for the description of
   * this function, and `include/server/message.hh` for the values that can be
   * used to filled `reply`. See an example below (you can remove them and write
   * your own code).
   */
  reply->reply_status_ = ReplyStatus::OK;
  reply->guess_results_[0] = GuessResult::CORRECT;
}

void WordleServer::Run(const uint16_t port, const uint32_t thread_num) {
  server_ = std::make_unique<rpc::server>(port);

  server_->bind("Guess", [&](const GuessArgs& args) {
    GuessReply reply;
    Guess(args, &reply);
    RpcReply rpc_reply;
    rpc_reply.reply_status_ = reply.reply_status_;
    for (auto& guess_result : reply.guess_results_) {
      rpc_reply.guess_results_.emplace_back(guess_result);
    }
    return rpc_reply;
  });
  server_->bind("Connect", [&]() { return Connect(); });

  server_->async_run(thread_num);
  std::cerr << "Wordle running on port " << port << " with " << thread_num << " threads.\n";

  while (true) {
  }  // run forever
}