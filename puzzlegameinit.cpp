#include "puzzlegameinit.h"

#include <QFile>
#include <QTextStream>

#include "rotatepuzzlegame.h"

RotatePuzzleGame* PuzzleGameInit::initRotatePuzzleGame(int index,
                                                       int type,
                                                       int minSteps)
{
  // Two arrays to store infomation of the start state and end state
  int *ballIndex = new int [61];
  int *toBeIndex = new int [61];

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
    return NULL;
  QTextStream in(&file);
  for (int i = 0; i < 61; ++i)
  {
    in >> ballIndex[i];
  }
  for (int i = 0; i < 61; ++i)
  {
    in >> toBeIndex[i];
  }

  // Create the game
  RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex,
                                                      index, type, minSteps);

  // Release space
  delete ballIndex;
  delete toBeIndex;

  return puzzleGame;
}
