#include "othergameinit.h"

#include "abstractpixmapwidget.h"
#include "classicgamewidget.h"
#include "endlessgamewidget.h"
#include "timinggamewidget.h"
#include "gamerecord.h"
#include "ball.h"

#include <QDebug>

extern GameRecord gameRecord;

// The names of the files
static const char* fileNames[] = {
  "SwapClassic",
  "RotateClassic",
  "SwapEndless",
  "RotateEndless"
};

// Convert a ball to an integer
int ballToInt(Ball *ball)
{
  if (ball == NULL)
    return 0xFFFF;
  return ((int) ball->getColor()) +
          (ball->getLocked() ? 0x8000 : 0x0000);
}

// Convert an integer to a ball
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
  // Create the correct game
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
  // Create the file if neccessary
  if (!gameRecord.exists("OtherGameHighest"))
  {
    int tmp[] = {0,0,0,0,0,0};
    gameRecord.createFile("OtherGameHighest", 6);
    gameRecord.writeDataArr("OtherGameHighest", tmp, 6);
  }
  return gameRecord.readData("OtherGameHighest", index);
}

void OtherGameInit::setHighest(int index, int score)
{
  // Create the file if neccessary
  if (!gameRecord.exists("OtherGameHighest"))
  {
    int tmp[] = {0,0,0,0,0,0};
    gameRecord.createFile("OtherGameHighest", 6);
    gameRecord.writeDataArr("OtherGameHighest", tmp, 6);
  }
  gameRecord.writeData("OtherGameHighest", index, score);
}

void OtherGameInit::testHighest(int index, int score)
{
  // Record the score if neccessary
  if (getHighest(index) < score)
    setHighest(index, score);
}

OtherGameRecord *OtherGameInit::loadOtherGame(int index)
{
  if (index > 3)
    return NULL;

  // The record
  OtherGameRecord *result = new OtherGameRecord();
  result->highestScore = getHighest(index);

  // Set the values to default if neccessary
  if (!gameRecord.exists(fileNames[index]))
  {
    result->currentLevel = 1;
    result->minScore = 0;
    result->currentScore = 0;
    result->maxScore = 30;
    result->flame = 0;
    result->star = 0;
    result->balls = NULL;
  }
  // Set the values to the values in the file
  else
  {
    int *data;
    int size;
    gameRecord.readDataArr(fileNames[index], data, size);
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

void OtherGameInit::saveOtherGame(OtherGameRecord *record,
                                  int index,
                                  int ballCount)
{
  if (index > 3)
    return;

  // Test the highest score
  if (getHighest(index) < record->currentScore)
    setHighest(index, record->currentScore);

  // Make the data array
  int *data = new int[6 + ballCount];
  int count = 0;
  data[count++] = record->currentLevel;
  data[count++] = record->minScore;
  data[count++] = record->currentScore;
  data[count++] = record->maxScore;
  data[count++] = record->flame;
  data[count++] = record->star;

  if (record->balls)
    for (int i = 0;i < ballCount;++i)
      data[count++] = ballToInt(record->balls[i]);
  else
    for (int i = 0;i < ballCount;++i)
      data[count++] = ballToInt(NULL);

  // Write the data to the file
  gameRecord.writeDataArr(fileNames[index], data, 6 + ballCount);

  // Release space
  delete [] data;
}

void OtherGameInit::clearGame(int index)
{
  // Remove the file of the game
  gameRecord.remove(fileNames[index]);
}
