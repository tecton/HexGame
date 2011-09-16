/*
** Class used to initialize a puzzle game.
*/

#ifndef PUZZLEGAMEINIT_H
#define PUZZLEGAMEINIT_H

// Forward declaration
class RotatePuzzleGame;

class PuzzleGameInit
{
public:
  // Init a game with the index, type, and min step
  static RotatePuzzleGame* initRotatePuzzleGame(int index,
                                                int type,
                                                int minSteps);
};

#endif // PUZZLEGAMEINIT_H
