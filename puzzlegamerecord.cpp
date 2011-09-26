#include "puzzlegamerecord.h"

#include <QString>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include "gamerecord.h"

extern GameRecord gameRecord;

void PuzzleGameRecord::getColorIndexes(int type,
                                       int index,
                                       int *&ballIndex,
                                       int *&toBeIndex,
                                       int& count)
{
  // Two arrays to store infomation of the start state and end state
  ballIndex = new int [61];
  toBeIndex = new int [61];
  count = 61;

  // Calculate the file name
  QString filename = ":/puzzlegames/";

  // choose which type to load
  switch (type)
  {
  case 0:
    filename += "exchange";
    break;
  case 1:
    filename += "exchange_plus";
    break;
  case 2:
    filename += "unite";
    break;
  case 3:
    filename += "unite_plus";
    break;
  case 4:
    filename += "lock";
    break;
  case 5:
    filename += "lock_plus";
    break;
  }
  filename = QObject::tr("%1%2").arg(filename).arg(index + 1);

  // Read the data of the balls from the file
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    ballIndex = NULL;
    toBeIndex = NULL;
    count = 0;
  }
  QTextStream in(&file);
  for (int i = 0; i < 61; ++i)
  {
    in >> ballIndex[i];
  }
  for (int i = 0; i < 61; ++i)
  {
    in >> toBeIndex[i];
  }
}

int PuzzleGameRecord::minSteps(int type, int index)
{
  int result = -1;
  int *array;
  int count;
  minSteps(type, array, count);
  if (index < count && index >= 0)
    result = array[index];
  delete [] array;
  return result;
}

void PuzzleGameRecord::testLeastSteps(int type, int index, int step)
{
  int min = minSteps(type, index);
  int count;
  if (min == -1 || step < min)
  {
    QString fileName;
    switch (type / 2)
    {
    case 0:
      fileName = "Exchange";
      count = 8;
      break;
    case 1:
      fileName = "Unite";
      count = 10;
      break;
    case 2:
      fileName = "Lock";
      count = 20;
      break;
    default:
      return;
      break;
    }

    // examine record
    if (!gameRecord.exists(fileName))
    {
      gameRecord.createFile(fileName, count);
      int *initialSteps = new int[count];
      for (int i = 0;i < count;++i)
        initialSteps[i] = -1;
      gameRecord.writeDataArr(fileName, initialSteps, count);
      delete [] initialSteps;
    }

    gameRecord.writeData(fileName,
                         (type % 2 == 1) ? count / 2 + index : index,
                         step);
  }
}

void PuzzleGameRecord::minSteps(int type, int *&array, int& count)
{
  QString fileName;
  switch (type / 2)
  {
  case 0:
    fileName = "Exchange";
    count = 8;
    break;
  case 1:
    fileName = "Unite";
    count = 10;
    break;
  case 2:
    fileName = "Lock";
    count = 20;
    break;
  default:
    array = NULL;
    count = 0;
    return;
    break;
  }

  // examine record
  if (!gameRecord.exists(fileName))
  {
    gameRecord.createFile(fileName, count);
    int *initialSteps = new int[count];
    for (int i = 0;i < count;++i)
      initialSteps[i] = -1;
    gameRecord.writeDataArr(fileName, initialSteps, count);
    delete [] initialSteps;
  }
  gameRecord.readDataArr(fileName, array, count);
  if (type % 2 == 1)
  {
    for (int i = 0;i < count / 2;++i)
      array[i] = array[count / 2 + i];
  }
  count = count / 2;
}

int PuzzleGameRecord::clearedStages(int type)
{
  int result = 0;
  int *array;
  int count;
  minSteps(type, array, count);
  for (int i = 0;i < count;++i)
    if (array[i] >= 0)
      ++result;
  delete [] array;
  return result;
}

int PuzzleGameRecord::clearedStages()
{
  int result = 0;
  for (int i = 0;i < 6;++i)
    result += clearedStages(i);
  return result;
}

int PuzzleGameRecord::totalStages(int type)
{
  int *array;
  int count;
  minSteps(type, array, count);
  delete [] array;
  return count;
}

int PuzzleGameRecord::totalStages()
{
  int result = 0;
  for (int i = 0;i < 6;++i)
    result += totalStages(i);
  return result;
}



