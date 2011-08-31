#ifndef PUZZLEGAMEINIT_H
#define PUZZLEGAMEINIT_H

class RotatePuzzleGame;

class PuzzleGameInit
{
public:
  static RotatePuzzleGame* initRotatePuzzleGame(int index,
                                                int type,
                                                int minSteps);
};

#endif // PUZZLEGAMEINIT_H
