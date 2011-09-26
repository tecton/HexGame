#include "puzzlegameinit.h"

#include "rotatepuzzlegame.h"
#include "puzzlegamerecord.h"

RotatePuzzleGame* PuzzleGameInit::initRotatePuzzleGame(int index,
                                                       int type)
{
  int *ballIndex;
  int *toBeIndex;
  int tmp;

  PuzzleGameRecord::getColorIndexes(type, index, ballIndex, toBeIndex, tmp);

  int minSteps = PuzzleGameRecord::minSteps(type, index);

  // Create the game
  RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex,
                                                      toBeIndex,
                                                      index,
                                                      type,
                                                      minSteps);

  // Release space
  delete [] ballIndex;
  delete [] toBeIndex;

  return puzzleGame;
}
