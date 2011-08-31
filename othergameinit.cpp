#include "othergameinit.h"

#include "abstractpixmapwidget.h"
#include "classicgamewidget.h"
#include "endlessgamewidget.h"
#include "timinggamewidget.h"
#include "gamerecord.h"
#include "ball.h"

#include <QDebug>

extern GameRecord record;

static const char* fileNames[] = {
  "SwapClassic",
  "RotateClassic",
  "SwapEndless",
  "RotateEndless"
};

int ballToInt(Ball *ball)
{
  if (ball == NULL)
    return 0xFFFF;
  return ((int) ball->getColor()) +
          (ball->getLocked() ? 0x8000 : 0x0000);
}

Ball *intToBall(int value)
{
  if (value == 0xFFFF)
    return NULL;
  Ball::Color color = (Ball::Color)(value & 0x7FFF);
  bool locked = (value & 0x8000) == 0x8000;
  Ball *result = new Ball(color);
  result->setLocked(locked);
  return result;

}

AbstractPixmapWidget *OtherGameInit::initOtherGame(AbstractRule::Gesture gesture,
                                                   int type)
{
  if (type == 0)
    return new ClassicGameWidget(gesture);
  else if (type == 1)
    return new EndlessGameWidget(gesture);
  else if (type == 2)
    return new TimingGameWidget(gesture);
  return 0;
}

int OtherGameInit::getHighest(int index)
{
  if (!record.exists("OtherGameHighest"))
  {
    int tmp[] = {0,0,0,0,0,0};
    record.createFile("OtherGameHighest", 6);
    record.writeDataArr("OtherGameHighest", tmp, 6);
  }
  return record.readData("OtherGameHighest", index);
}

void OtherGameInit::setHighest(int index, int score)
{
  if (!record.exists("OtherGameHighest"))
  {
    int tmp[] = {0,0,0,0,0,0};
    record.createFile("OtherGameHighest", 6);
    record.writeDataArr("OtherGameHighest", tmp, 6);
  }
  record.writeData("OtherGameHighest", index, score);
}

void OtherGameInit::testHighest(int index, int score)
{
  if (getHighest(index) < score)
    setHighest(index, score);
}

OtherGameRecord *OtherGameInit::loadOtherGame(int index)
{
  if (index > 3)
    return NULL;
  OtherGameRecord *result = new OtherGameRecord();

  result->highestScore = getHighest(index);
  if (!record.exists(fileNames[index]))
  {
    result->currentLevel = 1;
    result->minScore = 0;
    result->currentScore = 0;
    result->maxScore = 30;
    result->flame = 0;
    result->star = 0;
    result->balls = NULL;
  }
  else
  {
    int *data;
    int size;
    record.readDataArr(fileNames[index], data, size);
    int count = 0;
    result->currentLevel = data[count++];
    result->minScore = data[count++];
    result->currentScore = data[count++];
    result->maxScore = data[count++];
    result->flame = data[count++];
    result->star = data[count++];
    result->balls = new Ball *[size - count];
    int ballCount = count;
    for (int i = 0;i < size - ballCount;++i)
      result->balls[i] = intToBall(data[count++]);
  }
  return result;
}

void OtherGameInit::saveOtherGame(OtherGameRecord *gameRecord,
                                  int index,
                                  int ballCount)
{
  if (index > 3)
    return;

  if (getHighest(index) < gameRecord->currentScore)
    setHighest(index, gameRecord->currentScore);

  int *data = new int[6 + ballCount];
  int count = 0;
  data[count++] = gameRecord->currentLevel;
  data[count++] = gameRecord->minScore;
  data[count++] = gameRecord->currentScore;
  data[count++] = gameRecord->maxScore;
  data[count++] = gameRecord->flame;
  data[count++] = gameRecord->star;

  if (gameRecord->balls)
    for (int i = 0;i < ballCount;++i)
      data[count++] = ballToInt(gameRecord->balls[i]);
  else
    for (int i = 0;i < ballCount;++i)
      data[count++] = ballToInt(NULL);

  record.writeDataArr(fileNames[index], data, 6 + ballCount);
  delete [] data;
}

void OtherGameInit::clearGame(int index)
{
  record.remove(fileNames[index]);
}
