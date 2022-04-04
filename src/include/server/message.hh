#ifndef MSG_HH
#define MSG_HH

#include <vector>

#include "common/constant.hh"
#include "rpc/msgpack.hpp"

using UserId = uint32_t;

enum class GuessResult {
  CORRECT,
  PRESENT,
  ABSENT,
};

enum class ReplyStatus {
  OK,
  WORD_NOT_FOUND,
  INVALID_USER,
  WON,
  LOST,
};

MSGPACK_ADD_ENUM(GuessResult); // NOLINT
MSGPACK_ADD_ENUM(ReplyStatus); // NOLINT

/**
 * user_id: Unique identifier to identify the user making the request
 * guesses: The five characters that user guesses
 */
struct GuessArgs {
  UserId user_id_;
  char guess_[WORD_LENGTH + 1];
  MSGPACK_DEFINE_ARRAY(user_id_, guess_); // NOLINT
};

/**
 * reply_status: Indicate whether the guess request is successful
 * guess_results: If the request is valid, each entry in it indicates the result
 * for the corresponding character
 */
struct GuessReply {
  ReplyStatus reply_status_;
  GuessResult guess_results_[WORD_LENGTH];
};

struct RpcReply {
  ReplyStatus reply_status_;
  std::vector<GuessResult> guess_results_;
  MSGPACK_DEFINE_ARRAY(reply_status_, guess_results_); // NOLINT
};

#endif