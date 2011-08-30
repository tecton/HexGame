#ifndef OTHERGAMEINIT_H
#define OTHERGAMEINIT_H

#include "abstractrule.h"

class AbstractPixmapWidget;
class AbstractRule;
class Ball;

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

class OtherGameInit
{
public:
  static AbstractPixmapWidget *initOtherGame(AbstractRule::Gesture gesture,
                                             int type);
  static int getHighest(int index);
  static void setHighest(int index, int score);
  static OtherGameRecord *loadOtherGame(int index);
  static void saveOtherGame(OtherGameRecord *record,
                            int index,
                            int ballCount);
  static void clearGame(int index);
};

#endif // OTHERGAMEINIT_H
