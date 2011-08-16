#include "puzzlegameinit.h"

#include <QFile>
#include <QTextStream>

#include "rotatepuzzlegame.h"

RotatePuzzleGame* PuzzleGameInit::initRotatePuzzleGame(int index,
                                                       int type)
{
  int *ballIndex = new int [61];
  int *toBeIndex = new int [61];
  QString filename = ":/abcde/";
//  if (type == 0)
//    filename = ":/abcde/exchange";
//  if (type == 1)
//    filename = ":/abcde/unite";
//  if (type == 2)
//    filename = ":/abcde/lock_plus";
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
  RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex,
                                                      index, type);
  delete ballIndex;
  delete toBeIndex;
  return puzzleGame;
}
