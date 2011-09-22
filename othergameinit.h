#ifndef OTHERGAMEINIT_H
#define OTHERGAMEINIT_H

// File must include
#include "abstractrule.h"

// Forward declaration
class AbstractPixmapWidget;
class AbstractRule;
class Ball;

/**
 * @brief A class to record the infomation of a game.
 */
class OtherGameRecord
{
public:
  Ball **balls;
  int highestScore;
  int currentLevel;
  int minScore;
  int currentScore;
  int maxScore;
  int flame;
  int star;
};

/**
 * @brief A class to init other games.
 *
 * In fact, there are some functions to connect the game with the file.
 */
class OtherGameInit
{
public:
  /**
   * @brief Init a game with a gesture and type of the game.
   */
  static AbstractPixmapWidget *initOtherGame(AbstractRule::Gesture gesture,
                                             int type);

  /**
   * @brief Get the highest score of the game with index given.
   */
  static int getHighest(int index);

  /**
   * @brief Set the highest score of the game with index given.
   */
  static void setHighest(int index, int score);

  /**
   * @brief Test the highest score of the game with index given, may use
   *  setHighest.
   */
  static void testHighest(int index, int score);

  /**
   * @brief Get the record of a game with index given.
   */
  static OtherGameRecord *loadOtherGame(int index);

  /**
   * @brief Save the record of a game with index given and the count of the
   *  balls.
   */
  static void saveOtherGame(OtherGameRecord *record,
                            int index,
                            int ballCount);

  /**
   * @brief Clear the record of a game with index given.
   */
  static void clearGame(int index);
};

#endif // OTHERGAMEINIT_H
