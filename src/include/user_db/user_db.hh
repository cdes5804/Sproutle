#ifndef USER_DB_HH
#define USER_DB_HH

#include <mutex>
#include <unordered_map>

#include "common/constant.hh"
#include "server/message.hh"

struct UserInfo {
  UserId user_id_;
  int32_t guess_count_;
  char answer_[WORD_LENGTH + 1];
};

class UserDB {
 private:
  UserInfo users_[MAX_USER_NUM];
  std::mutex mu_;

 public:
  /**
   * Create a new user, set the answer for the user, and assign a unique user id
   * to it.
   * @param answer_for_user A string containing the answer for the newly created
   * user.
   * @return The id of the newly created user
   */
  UserId CreateNewUser(const char answer_for_user[]);

  /**
   * Get the answer for the given user.
   * @param user_id The id of the user to process
   * @param answer_for_user The buffer to put the answer of the user. If the
   * user does not exits, put empty string
   */
  void GetUserAnswer(UserId user_id, char answer_for_user[]);

  /**
   * Get the guess count of the given user by one.
   * @param user_id The id of the user to process
   * @return -1 if the user is not found, the guess count otherwise
   */
  int32_t GetUserGuessCount(UserId user_id);

  /**
   * Set the guess count of the given user.
   * @param user_id The id of the user to process
   */
  void SetUserGuessCount(UserId user_id, int32_t guess_count);

  /**
   * Remove the user in databse.
   * @param user_id The id of the user to remove
   * @return true if the removal is successful, false otherwise
   */
  bool RemoveUserFromDB(UserId user_id);
};

#endif