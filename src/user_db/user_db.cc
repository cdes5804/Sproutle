#include <cstring>

#include "user_db/user_db.hh"

UserId UserDB::CreateNewUser(const char answer_for_user[]) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Create a new user and set the word he/she has to guess to
   * `answer_for_user`. You should fill the information for the new user in
   * `users_`, which is an array of UserInfo (see `include/user_db/user_db.hh`
   * for its structure). Finally, return the user id for the newly create user.
   */

  return 0;
}

void UserDB::GetUserAnswer(const UserId user_id, char answer_for_user[]) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Return the answer for the user with `user_id` by putting the string in
   * `answer_for_user`. If there is no user with `user_id`, set
   * `answer_for_user` to empty string.
   */
}

int32_t UserDB::GetUserGuessCount(const UserId user_id) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Return the guess count for the user with `user_id`.
   * If there is no user with `user_id`, return -1.
   */

  return -1;
}

void UserDB::SetUserGuessCount(const UserId user_id, const int32_t guess_count) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Set the guess count for the user with `user_id` to `guess_count`.
   * If there is no user with `user_id`, do nothing.
   */
}

bool UserDB::RemoveUserFromDB(const UserId user_id) {
  /* Do not remove the following line */
  std::scoped_lock lock(mu_);

  /**
   * TODO:
   * Remove the user with `user_id` from DB.
   * There are many ways to remove a user, you have to make sure the user cannot
   * be found anymore after his/her removal.
   */

  return true;
}