#ifndef SERVER_HH
#define SERVER_HH

#include <memory>
#include <mutex>

#include "dictionary/dictionary.hh"
#include "rpc/server.h"
#include "server/message.hh"
#include "user_db/user_db.hh"

class WordleServer {
private:
  std::unique_ptr<Dictionary> dictionary_;
  std::unique_ptr<UserDB> user_db_;
  std::unique_ptr<rpc::server> server_;
  const int32_t guess_limit_ = 6;
  std::mutex mu_;

public:
  explicit WordleServer(std::unique_ptr<Dictionary> dictionary,
                        std::unique_ptr<UserDB> user_db);

  /**
   * Handler for the connect requests. Called when a new player wish to join the
   * game.
   * @return A unique user id
   */
  UserId Connect();

  /**
   * Handler for guess requests.
   * @param args The content of the guess requests
   * @param reply A pointer to the reply object, which should be filled by the
   * function
   */
  void Guess(const GuessArgs &args, GuessReply *reply);

  /**
   * Start the Wordle game and listen for incoming guess requests.
   * @param port The port to listen to
   * @param thread_num The number of thread used by the server, which
   * corresponds to the maximum number of parallel requests available
   */
  void Run(uint16_t port, uint32_t thread_num);
};

#endif