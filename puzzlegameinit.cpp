#include "puzzlegameinit.h"

#include <QFile>
#include <QTextStream>

#include "rotatepuzzlegame.h"

RotatePuzzleGame* PuzzleGameInit::initRotatePuzzleGame(int index,
                                                       int type)
{
  int *ballIndex = new int [61];
  int *toBeIndex = new int [61];
  QString filename;
  if (type == 0)
    filename = ":/stages/exchange";
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
  RotatePuzzleGame *puzzleGame = new RotatePuzzleGame(ballIndex, toBeIndex);
  delete ballIndex;
  delete toBeIndex;
  return puzzleGame;
}
