#include "statistic.h"

#include "gamerecord.h"

extern GameRecord gameRecord;

#define FILE_NAME "Statistic"

void Statistic::changeStatistic(Statistic::StatisticType type,
                                int value,
                                bool add)
{
  // Create the file if neccessary
  if (!gameRecord.exists(FILE_NAME))
  {
    int tmp[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    gameRecord.createFile(FILE_NAME, 14);
    gameRecord.writeDataArr(FILE_NAME, tmp, 14);
  }
  int original = gameRecord.readData(FILE_NAME, type);
  int to = add ? original + value : value;
  gameRecord.writeData(FILE_NAME, type, to);
  emit statisticChanged(type, original, to);
}

int Statistic::getStatistic(StatisticType type)
{
  // Create the file if neccessary
  if (!gameRecord.exists(FILE_NAME))
  {
    int tmp[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    gameRecord.createFile(FILE_NAME, 14);
    gameRecord.writeDataArr(FILE_NAME, tmp, 14);
  }
  return gameRecord.readData(FILE_NAME, type);
}
