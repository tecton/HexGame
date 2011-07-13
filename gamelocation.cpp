
#include "gamelocation.h"
#include <qmath.h>


int row[] = {
//                     0,  0,  0,  0,  0,  0,
//                     1,  1,  1,  1,  1,  1,  1,
//                     2,  2,  2,  2,  2,  2,  2,  2,
//                     3,  3,  3,  3,  3,  3,  3,  3,  3,
//                     4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
//                     5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
//                     6,  6,  6,  6,  6,  6,  6,  6,  6,  6,
//                     7,  7,  7,  7,  7,  7,  7,  7,  7,
//                     8,  8,  8,  8,  8,  8,  8,  8,
//                     9,  9,  9,  9,  9,  9,  9,
//                    10, 10, 10, 10, 10, 10
          0,  0,  0,  0,  0,
        1,  1,  1,  1,  1,  1,
      2,  2,  2,  2,  2,  2,  2,
    3,  3,  3,  3,  3,  3,  3,  3,
  4,  4,  4,  4,  4,  4,  4,  4,  4,
    5,  5,  5,  5,  5,  5,  5,  5,
      6,  6,  6,  6,  6,  6,  6,
        7,  7,  7,  7,  7,  7,
          8,  8,  8,  8,  8
};




int col[] = {
//                        5,  7,  9, 11, 13, 15,
//                        4,  6,  8, 10, 12, 14, 16,
//                        3,  5,  7,  9, 11, 13, 15, 17,
//                        2,  4,  6,  8, 10, 12, 14, 16, 18,
//                        1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
//                        0,  2,  4,  6,  8, 10, 12, 14, 16, 18, 20,
//                        1,  3,  5,  7,  9, 11, 13, 15, 17, 19,
//                        2,  4,  6,  8, 10, 12, 14, 16, 18,
//                        3,  5,  7,  9, 11, 13, 15, 17,
//                        4,  6,  8, 10, 12, 14, 16,
//                        5,  7,  9, 11, 13, 15
          4,  6,  8, 10, 12,
        3,  5,  7,  9, 11, 13,
      2,  4,  6,  8, 10, 12, 14,
    1,  3,  5,  7,  9, 11, 13, 15,
  0,  2,  4,  6,  8, 10, 12, 14, 16,
    1,  3,  5,  7,  9, 11, 13, 15,
      2,  4,  6,  8, 10, 12, 14,
        3,  5,  7,  9, 11, 13,
          4,  6,  8, 10, 12
};

int positionToIndex[] = {
  -1, -1, -1, -1,  0,  0,  1,  1,  2,  2,  3,  3,  4,  4, -1, -1, -1, -1,
  -1, -1, -1,  5,  5,  6,  6,  7,  7,  8,  8,  9,  9, 10, 10, -1, -1, -1,
  -1, -1, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, -1, -1,
  -1, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25, -1,
  26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, 33, 33, 34, 34,
  -1, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 40, 40, 41, 41, 42, 42, -1,
  -1, -1, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, -1, -1,
  -1, -1, -1, 50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 55, 55, -1, -1, -1,
  -1, -1, -1, -1, 56, 56, 57, 57, 58, 58, 59, 59, 60, 60, -1, -1, -1, -1
};

int itemCountInAChain[] = {
  1, 6, 12, 18, 24
};

int originalChain[] = {
  30,
  29,             21,             22,             31,             39,             38,
  28, 20,         13, 14,         15, 23,         32, 40,         47, 46,         45, 37,
  27, 19, 12,      6,  7,  8,      9, 16, 24,     33, 41, 48,     54, 53, 52,     51, 44, 36,
  26, 18, 11,  5,  0,  1,  2,  3,  4, 10, 17, 25, 34, 42, 49, 55, 60, 59, 58, 57, 56, 50, 43, 35
};

int itemBelongsToChain[] = {
          4,  4,  4,  4,  4,
        4,  3,  3,  3,  3,  4,
      4,  3,  2,  2,  2,  3,  4,
    4,  3,  2,  1,  1,  2,  3,  4,
  4,  3,  2,  1,  0,  1,  2,  3,  4,
    4,  3,  2,  1,  1,  2,  3,  4,
      4,  3,  2,  2,  2,  3,  4,
        4,  3,  3,  3,  3,  4,
          4,  4,  4,  4,  4
};

int indexToIsJoint[] = {
          1,  0,  0,  0,  1,
        0,  1,  0,  0,  1,  0,
      0,  0,  1,  0,  1,  0,  0,
    0,  0,  0,  1,  1,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,  1,
    0,  0,  0,  1,  1,  0,  0,  0,
      0,  0,  1,  0,  1,  0,  0,
        0,  1,  0,  0,  1,  0,
          1,  0,  0,  0,  1
};

int columnOfIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return 0;
  return col[index];
}

int rowOfIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return 0;
  return row[index];
}

QPointF positionOfIndex(int index)
{
  int r = rowOfIndex(index);
  int c = columnOfIndex(index);

  qreal x = LOCATION_GAME_BOARD_ITEM_X_FROM +
            c * LOCATION_GAME_BOARD_ITEM_X_INTERVAL;
  qreal y = LOCATION_GAME_BOARD_ITEM_Y_FROM +
            r * LOCATION_GAME_BOARD_ITEM_Y_INTERVAL;
  return QPointF(x, y);
}

QVector<QVector<int> > chainsFromCenter;

const QVector<QVector<int> >& chains()
{
  if (chainsFromCenter.isEmpty())
  {
    int currentIndexInOriginalChain = 0;
    for (int i = 0;i < CHAIN_NUMBER;++i)
    {
      QVector<int> currentChain;
      for (int j = 0;j < itemCountInAChain[i];++j)
        currentChain.push_back(originalChain[currentIndexInOriginalChain++]);
      chainsFromCenter.push_back(currentChain);
    }
  }
  return chainsFromCenter;
}

QVector<int> chainOfIndex(int index, bool full)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return QVector<int>();
  const QVector<QVector<int> >& allChains = chains();
  const QVector<int>& fullChain = allChains.at(itemBelongsToChain[index]);
  QVector<int> result;
  for (int i = 0;i < fullChain.size();++i)
  {
    result.push_back(fullChain.at(i));
    if ((!full) && fullChain.at(i) == index)
      break;
  }
  return result;
}


QVector<int> chainOfIndex(int from,
                          int to,
                          bool counterclockwise)
{
  if (from < 0 || from >= TOTAL_ITEM_NUMBER)
    return QVector<int>();
  if (to < 0 || to >= TOTAL_ITEM_NUMBER)
    return QVector<int>();

  const QVector<QVector<int> >& allChains = chains();
  const QVector<int>& fullChain = allChains.at(itemBelongsToChain[from]);
  QVector<int> counterclockwiseResult;

  int pos = fullChain.indexOf(from, 0);
  bool resultFound = false;
  for (int i = pos;i < fullChain.size();++i)
  {
    counterclockwiseResult.push_back(fullChain.at(i));
    if (fullChain.at(i) == to)
    {
      resultFound = true;
      break;
    }
  }
  if (!resultFound)
  {
    for (int i = 0;i < pos;++i)
    {
      counterclockwiseResult.push_back(fullChain.at(i));
      if (fullChain.at(i) == to)
        resultFound = true;
        break;
    }
  }

  if (counterclockwiseResult.size() > fullChain.size() / 2 &&
      !counterclockwise)
  {
    QVector<int> result(counterclockwiseResult.size());
    for (int i = 0;i < counterclockwiseResult.size();++i)
      result[i] = counterclockwiseResult[counterclockwiseResult.size() - 1 - i];
    return result;
  }
  return counterclockwiseResult;
}


int indexOfPosition(QPointF position)
{
  int c = (position.x() - LOCATION_GAME_BOARD_ITEM_X_FROM) *
          COLUMN_NUMBER /
          (LOCATION_GAME_BOARD_ITEM_X_TO - LOCATION_GAME_BOARD_ITEM_X_FROM);
  int r = (position.y() - LOCATION_GAME_BOARD_ITEM_Y_FROM) *
          ROW_NUMBER /
          (LOCATION_GAME_BOARD_ITEM_Y_TO - LOCATION_GAME_BOARD_ITEM_Y_FROM);
  if (c < 0 || c >= COLUMN_NUMBER || r < 0 || r > ROW_NUMBER)
    return -1;
  int index = positionToIndex[r * COLUMN_NUMBER + c];
  if (distanceOfTwoPoints(position, positionOfIndex(index)) > LOCATION_GAME_BOARD_ITEM_X_INTERVAL)
    return -1;
  return index;
}

int indexOfPosition(int row, int column)
{
  return positionToIndex[row * COLUMN_NUMBER + column];
}

int indexOfMousePosition(QPointF position)
{
  position.setX(position.x() - ITEM_WIDTH / 2);
  position.setY(position.y() - ITEM_HEIGHT / 2);
  return indexOfPosition(position);
}

qreal distanceOfTwoPoints(QPointF p1, QPointF p2)
{
  return qSqrt(qPow(p1.x() - p2.x(), 2) + qPow(p1.y() - p2.y(), 2));
}

qreal angle(QPointF point, QPointF origin)
{
  qreal dx = point.x() - origin.x();
  qreal dy = origin.y() - point.y();
  qreal result = qAtan(qAbs(dy / dx));
  if (dx < 0 && dy >= 0)
    return PI - result;
  if (dx <= 0 && dy < 0)
    return PI + result;
  if (dx > 0 && dy < 0)
    return 2 * PI - result;
  return result;
}

QPointF calculatePosition(qreal a, qreal r, QPointF origin)
{
  qreal dx = qCos(a) * r;
  qreal dy = -qSin(a) * r;
  qreal x = dx + origin.x();
  qreal y = dy + origin.y();
  return QPointF(x, y);
}

qreal bridgeY(qreal bridgeX)
{
  if (bridgeX <= 0 || bridgeX >= 1)
    return 0;
  qreal dx = qAbs(0.5 - bridgeX);
  return qSqrt(1 - qPow(dx, 2)) - 0.8660;
}

bool isJoint(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return false;
  return indexToIsJoint[index];
}

int indexToLeftUp[] = {
           -1, -1, -1, -1, -1,
         -1,  0,  1,  2,  3,  4,
       -1, 5,  6,  7,  8,  9, 10,
     -1, 11, 12, 13, 14, 15, 16, 17,
   -1, 18, 19, 20, 21, 22, 23, 24, 25,
     26, 27, 28, 29, 30, 31, 32, 33,
       35, 36, 37, 38, 39, 40, 41,
         43, 44, 45, 46, 47, 48,
           50, 51, 52, 53, 54
};

int leftUpIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeftUp[index];
}

int indexToRightUp[] = {
           -1, -1, -1, -1, -1,
          0,  1,  2,  3,  4, -1,
        5,  6,  7,  8,  9, 10, -1,
     11, 12, 13, 14, 15, 16, 17, -1,
   18, 19, 20, 21, 22, 23, 24, 25, -1,
     27, 28, 29, 30, 31, 32, 33, 34,
       36, 37, 38, 39, 40, 41, 42,
         44, 45, 46, 47, 48, 49,
           51, 52, 53, 54, 55
};

int rightUpIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToRightUp[index];
}


int indexToRight[] = {

            1,  2,  3,  4, -1,
          6,  7,  8,  9, 10, -1,
       12, 13, 14, 15, 16, 17, -1,
     19, 20, 21, 22, 23, 24, 25, -1,
   27, 28, 29, 30, 31, 32, 33, 34, -1,
     36, 37, 38, 39, 40, 41, 42, -1,
       44, 45, 46, 47, 48, 49, -1,
         51, 52, 53, 54, 55, -1,
           57, 58, 59, 60 -1,
};

int rightIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToRight[index];
}

int indexToRightDown[] = {
            6,  7,  8,  9, 10,
         12, 13, 14, 15, 16, 17,
       19, 20, 21, 22, 23, 24, 25,
     27, 28, 29, 30, 31, 32, 33, 34,
   35, 36, 37, 38, 39, 40, 41, 42, -1,
     43, 44, 45, 46, 47, 48, 49, -1,
       50, 51, 52, 53, 54, 55, -1,
         56, 57, 58, 59, 60, -1,
           -1, -1, -1, -1, -1
};

int rightDownIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToRightDown[index];
}

int indexToLeftDown[] = {
          5,  6,  7,  8,  9,
       11, 12, 13, 14, 15, 16,
     18, 19, 20, 21, 22, 23, 24,
   26, 27, 28, 29, 30, 31, 32, 33,
 -1, 35, 36, 37, 38, 39, 40, 41, 42,
   -1, 43, 44, 45, 46, 47, 48, 49,
     -1, 50, 51, 52, 53, 54, 55,
       -1, 56, 57, 58, 59, 60,
         -1, -1, -1, -1, -1
};

int leftDownIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeftDown[index];
}

int indexToLeft[] = {
         -1,  0,  1,  2,  3,
       -1,  5,  6,  7,  8,  9,
     -1, 11, 12, 13, 14, 15, 16,
   -1, 18, 19, 20, 21, 22, 23, 24,
 -1, 26, 27, 28, 29, 30, 31, 32, 33,
   -1, 35, 36, 37, 38, 39, 40, 41,
     -1, 43, 44, 45, 46, 47, 48,
       -1, 50, 51, 52, 53, 54,
         -1, 56, 57, 58, 59,
};

int leftIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeft[index];
}

int indexHasAround[] = {
            0,  0,  0,  0,  0,
          0,  5,  6,  7,  8,  0,
        0, 11, 12, 13, 14, 15,  0,
      0, 18, 19, 20, 21, 22, 23,  0,
    0, 26, 27, 28, 29, 30, 31, 32,  0,
      0, 35, 36, 37, 38, 39, 40,  0,
        0, 43, 44, 45, 46, 47,  0,
          0, 50, 51, 52, 53,  0,
            0,  0,  0,  0,  0,
};

QVector<int> chainAroundIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return QVector<int>();
  QVector<int> result;
  int tmp = leftUpIndex(index);
  if (tmp >= 0)
    result.push_back(tmp);
  tmp = rightUpIndex(index);
  if (tmp >= 0)
    result.push_back(tmp);
  tmp = rightIndex(index);
  if (tmp >= 0)
    result.push_back(tmp);
  tmp = rightDownIndex(index);
  if (tmp >= 0)
    result.push_back(tmp);
  tmp = leftDownIndex(index);
  if (tmp >= 0)
    result.push_back(tmp);
  tmp = leftIndex(index);
  if (tmp >= 0)
    result.push_back(tmp);
  return result;
}

QVector<int> doubleChain(const QVector<int>& v)
{
  QVector<int> result = v;
  int size = result.size();
  for (int i = 0;i < size;++i)
    result.push_back(result[i]);
  return result;
}

//QVector<int> chainAroundIndex(int from,
//                              int to,
//                              int center,
//                              bool counterclockwise)
//{
//  if (from < 0 || from >= TOTAL_ITEM_NUMBER)
//    return QVector<int>();
//  if (to < 0 || to >= TOTAL_ITEM_NUMBER)
//    return QVector<int>();
//  if (center < 0 || center >= TOTAL_ITEM_NUMBER)
//    return QVector<int>();
//  QVector<int> originalChain = doubleChain(chainAroundIndex(center));
//  QVector<int> counterclockwiseResult;
//  int size = originalChain.size() / 2;

//  int fromPos = -1;
//  for (int i = 0;i < size;++i)
//    if (originalChain[i] == from)
//    {
//      fromPos = i;
//      break;
//    }
//  if (fromPos == -1)
//    return QVector<int>();

//  int toPos = -1;
//  for (int i = fromPos;i < 2 * size;++i)
//  {
//    counterclockwiseResult.push_back(originalChain[i]);
//    if (originalChain[i] == to)
//    {
//      toPos = i;
//      break;
//    }
//  }
//  if (toPos == -1)
//    return QVector<int>();

//  if (!counterclockwise)
//  {
//    QVector<int> result(counterclockwiseResult.size());
//    for (int i = 0;i < counterclockwiseResult.size();++i)
//      result[i] = counterclockwiseResult[counterclockwiseResult.size() - 1 - i];
//    return result;
//  }
//  return counterclockwiseResult;
//}

int indexToCanBeRotateCenter[] = {
            0,  0,  0,  0,  0,
          0,  1,  1,  1,  1,  0,
        0,  1,  1,  1,  1,  1,  0,
      0,  1,  1,  1,  1,  1,  1,  0,
    0,  1,  1,  1,  1,  1,  1,  1,  0,
      0,  1,  1,  1,  1,  1,  1,  0,
        0,  1,  1,  1,  1,  1,  0,
          0,  1,  1,  1,  1,  0,
            0,  0,  0,  0,  0
};

bool canBeRotateCenter(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return false;
  return indexToCanBeRotateCenter[index] == 1;
}

int indexToFirstLeftUpIndex[] = {
            0,  1,  2,  3,  4,
          5,  0,  1,  2,  3,  4,
       11,  5,  0,  1,  2,  3,  4,
     18, 11,  5,  0,  1,  2,  3,  4,
   26, 18, 11,  5,  0,  1,  2,  3,  4,
     26, 18, 11,  5,  0,  1,  2,  3,
       26, 18, 11,  5,  0,  1,  2,
         26, 18, 11,  5,  0,  1,
           26, 18, 11,  5,  0
};

int indexToLastLeftUpIndex[] = {
           60, 55, 49, 42, 34,
         59, 60, 55, 49, 42, 34,
       58, 59, 60, 55, 49, 42, 34,
     57, 58, 59, 60, 55, 49, 42, 34,
   56, 57, 58, 59, 60, 55, 49, 42, 34,
     56, 57, 58, 59, 60, 55, 49, 42,
       56, 57, 58, 59, 60, 55, 49,
         56, 57, 58, 59, 60, 55,
           56, 57, 58, 59, 60
};

int indexToFirstLeftDownIndex[] = {
           26, 35, 43, 50, 56,
         26, 35, 43, 50, 56, 57,
       26, 35, 43, 50, 56, 57, 58,
     26, 35, 43, 50, 56, 57, 58, 59,
   26, 35, 43, 50, 56, 57, 58, 59, 60,
     35, 43, 50, 56, 57, 58, 59, 60,
       43, 50, 56, 57, 58, 59, 60,
         50, 56, 57, 58, 59, 60,
           56, 57, 58, 59, 60
};


int indexToLastLeftDownIndex[] = {
            0,  1,  2,  3,  4,
          0,  1,  2,  3,  4, 10,
        0,  1,  2,  3,  4, 10, 17,
      0,  1,  2,  3,  4, 10, 17, 25,
    0,  1,  2,  3,  4, 10, 17, 25, 34,
      1,  2,  3,  4, 10, 17, 25, 34,
        2,  3,  4, 10, 17, 25, 34,
          3,  4, 10, 17, 25, 34,
            4, 10, 17, 25, 34
};

int rowToFirstIndex[] = {
  0,  5, 11, 18, 26, 35, 43, 50, 56
};

int rowToLastIndex[] = {
  4, 10, 17, 25, 34, 42, 49, 55, 60
};

int firstOfRow(int row)
{
  if (row < 0 && row >= ROW_NUMBER)
    return -1;
  return rowToFirstIndex[row];
}

int lastOfRow(int row)
{
  if (row < 0 && row >= ROW_NUMBER)
    return -1;
  return rowToLastIndex[row];
}

int firstOfLeftUp(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToFirstLeftUpIndex[index];
}

int firstOfLeftDown(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToFirstLeftDownIndex[index];
}

int lastOfLeftUp(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLastLeftUpIndex[index];
}

int lastOfLeftDown(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLastLeftDownIndex[index];
}

qreal distanceFromTheCenterWithTheAngle(qreal angle)
{
  while (angle < 0)
    angle += PI / 3;
  while (angle >= PI / 3)
    angle -= PI / 3;
  return LOCATION_GAME_BOARD_ITEM_X_INTERVAL * 2 *
         qSin(PI / 3) /
         qSin(2 * PI / 3 - angle);
}
