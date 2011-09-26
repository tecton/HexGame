#include "statistic.h"

#include "gamerecord.h"
#include "puzzlegamerecord.h"

extern GameRecord gameRecord;

#define FILE_NAME "Statistic"

void Statistic::changeStatistic(Statistic::StatisticType type,
                                int value,
                                bool add)
{
  if (type == RotatePuzzleFinished || type == RotatePuzzleTotal)
    return;

  // Create the file if neccessary
  if (!gameRecord.exists(FILE_NAME))
  {
    int tmp[] = {0,0,0,0,0,0,0,0,0,0,0,0};
    gameRecord.createFile(FILE_NAME, 12);
    gameRecord.writeDataArr(FILE_NAME, tmp, 12);
  }
  int original = gameRecord.readData(FILE_NAME, type);
  int to = add ? original + value : value;
  gameRecord.writeData(FILE_NAME, type, to);
  emit statisticChanged(type, original, to);
}

int Statistic::getStatistic(StatisticType type)
{
  if (type == RotatePuzzleFinished)
    return PuzzleGameRecord::clearedStages();
  if (type == RotatePuzzleTotal)
    return PuzzleGameRecord::totalStages();

  // Create the file if neccessary
  if (!gameRecord.exists(FILE_NAME))
  {
    int tmp[] = {0,0,0,0,0,0,0,0,0,0,0,0};
    gameRecord.createFile(FILE_NAME, 12);
    gameRecord.writeDataArr(FILE_NAME, tmp, 12);
  }
  return gameRecord.readData(FILE_NAME, type);
}
