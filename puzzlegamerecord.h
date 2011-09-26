#ifndef PUZZLEGAMERECORD_H
#define PUZZLEGAMERECORD_H

/**
 * @brief A class to deal with the records of puzzle games.
 */
class PuzzleGameRecord
{
public:
  /**
   * @brief Get the color indexes of a game.
   */
  static void getColorIndexes(int type,
                              int index,
                              int *&ballIndex,
                              int *&toBeIndex,
                              int& count);

  /**
   * @brief Get the min steps of a game.
   */
  static int minSteps(int type, int index);

  /**
   * @brief Set the min steps of a game if it's less than current value.
   */
  static void testLeastSteps(int type, int index, int step);

  /**
   * @brief Get the min steps of a kind of game.
   */
  static void minSteps(int type, int *&array, int& count);

  /**
   * @brief Get the count of stages cleard of a kind of game.
   */
  static int clearedStages(int type);

  /**
   * @brief Get the count of stages cleard of all kinds of games.
   */
  static int clearedStages();

  /**
   * @brief Get the count of stage of a kind of game.
   */
  static int totalStages(int type);

  /**
   * @brief Get the count of stage of all kinds of games.
   */
  static int totalStages();
};

#endif // PUZZLEGAMERECORD_H
