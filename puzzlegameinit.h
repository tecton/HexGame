#ifndef PUZZLEGAMEINIT_H
#define PUZZLEGAMEINIT_H

// Forward declaration
class RotatePuzzleGame;

/**
 * @brief A class used to initialize a puzzle game.
 */
class PuzzleGameInit
{
public:
  /**
   * @brief Init a game with the index, type, and min step.
   */
  static RotatePuzzleGame* initRotatePuzzleGame(int index,
                                                int type);
};

#endif // PUZZLEGAMEINIT_H
