#include "thirtysevengameboardinfo.h"

#include <qmath.h>
#include "gamemath.h"

// The width of the item
#define ITEM_WIDTH                              30

// The height of the item
#define ITEM_HEIGHT                             30

// The width of the item in a gesture
#define ITEM_GESTURE_R                          35

// The absolute position of game(I must draw a picture~~~)
#define LOCATION_GAME_VIEW_X_FROM               0
#define LOCATION_GAME_VIEW_Y_FROM               0
#define LOCATION_GAME_VIEW_X_TO                 800
#define LOCATION_GAME_VIEW_Y_TO                 500

#define LOCATION_GAME_BOARD_X_FROM              (LOCATION_GAME_VIEW_X_FROM + 240) // 200
#define LOCATION_GAME_BOARD_Y_FROM              10
#define LOCATION_GAME_BOARD_X_TO                (LOCATION_GAME_VIEW_X_TO - 20)    // 780
#define LOCATION_GAME_BOARD_Y_TO                (LOCATION_GAME_VIEW_Y_TO - 10)    // 490

#define LOCATION_GAME_BOARD_ITEM_X_FROM         (LOCATION_GAME_BOARD_X_FROM + ITEM_WIDTH)  // 220
#define LOCATION_GAME_BOARD_ITEM_Y_FROM         (LOCATION_GAME_BOARD_Y_FROM + ITEM_WIDTH)  // 40
#define LOCATION_GAME_BOARD_ITEM_X_TO           (LOCATION_GAME_BOARD_X_TO - 2 * ITEM_WIDTH) // 740
#define LOCATION_GAME_BOARD_ITEM_Y_TO           (LOCATION_GAME_BOARD_Y_TO - 2 * ITEM_HEIGHT) // 430

#define LOCATION_GAME_BOARD_ITEM_X_INTERVAL     (LOCATION_GAME_BOARD_ITEM_X_TO - \
                                                 LOCATION_GAME_BOARD_ITEM_X_FROM) / \
                                                 COLUMN_NUMBER
#define LOCATION_GAME_BOARD_ITEM_Y_INTERVAL     (LOCATION_GAME_BOARD_ITEM_Y_TO - \
                                                 LOCATION_GAME_BOARD_ITEM_Y_FROM) / \
                                                 ROW_NUMBER

// Total item number
#define TOTAL_ITEM_NUMBER                       37
// Total row number
#define ROW_NUMBER                              7
// Total column number
#define COLUMN_NUMBER                           14
// Total chain(circle) number
#define CHAIN_NUMBER                            4

// The offset of the scene and mouse
#define SCENE_TO_MOUSE_DX                       (ITEM_WIDTH / 2)
#define SCENE_TO_MOUSE_DY                       (ITEM_HEIGHT / 2)

// Width of the gameboard
int ThirtySevenGameBoardInfo::width()
{
  return LOCATION_GAME_VIEW_X_TO - LOCATION_GAME_VIEW_X_FROM;
}

// Height of the gameboard
int ThirtySevenGameBoardInfo::height()
{
  return LOCATION_GAME_VIEW_Y_TO - LOCATION_GAME_VIEW_Y_FROM;
}

// Radius of the ball
double ThirtySevenGameBoardInfo::ballR()
{
  return ITEM_WIDTH / 2;
}

// Number of the balls
int ThirtySevenGameBoardInfo::totalBallCounts()
{
  return TOTAL_ITEM_NUMBER;
}

int ThirtySevenGameBoardInfo::chainNumber()
{
  return CHAIN_NUMBER;
}

int col37[] = {
        3,  5,  7,  9,
      2,  4,  6,  8, 10,
    1,  3,  5,  7,  9, 11,
  0,  2,  4,  6,  8, 10, 12,
    1,  3,  5,  7,  9, 11,
      2,  4,  6,  8, 10,
        3,  5,  7,  9
};

// The column number of the item with the index
int ThirtySevenGameBoardInfo::columnOfIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return 0;
  return col37[index];
}


int row37[] = {
        0,  0,  0,  0,
      1,  1,  1,  1,  1,
    2,  2,  2,  2,  2,  2,
  3,  3,  3,  3,  3,  3,  3,
    4,  4,  4,  4,  4,  4,
      5,  5,  5,  5,  5,
        6,  6,  6,  6
};


// The row number of the item with the index

int ThirtySevenGameBoardInfo::rowOfIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return 0;
  return row37[index];
}

int itemCountInAChain37[] = {
  1, 6, 12, 18
};

int originalChain37[] = {
  18,
  17,         11,         12,         19,         25,         24,
  16, 10,      5,  6,      7, 13,     20, 26,     31,30,      29, 23,
  15,  9,  4,  0,  1,  2,  3,  8, 14, 21, 27, 32, 36, 35, 34, 33, 28, 22
};


// Left Top of the item with the index

QVector<QPointF> _positionOfIndex37;
QPointF ThirtySevenGameBoardInfo::positionOfIndex(int index)
{
  if (_positionOfIndex37.isEmpty())
  {
    for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
    {
      int r = rowOfIndex(i);
      int c = columnOfIndex(i);

      qreal x = LOCATION_GAME_BOARD_ITEM_X_FROM +
                c * LOCATION_GAME_BOARD_ITEM_X_INTERVAL;
      qreal y = LOCATION_GAME_BOARD_ITEM_Y_FROM +
                r * LOCATION_GAME_BOARD_ITEM_Y_INTERVAL;
      _positionOfIndex37.push_back(QPointF(x, y));
    }

  }
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return QPointF(LOCATION_GAME_BOARD_ITEM_X_FROM,
                   LOCATION_GAME_BOARD_ITEM_Y_FROM);
  return _positionOfIndex37[index];
}

// Center of the item with the index
QPointF ThirtySevenGameBoardInfo::centerPositionOfIndex(int index)
{
  QPointF result = positionOfIndex(index);
  result.setX(result.x() + ITEM_WIDTH / 2);
  result.setY(result.y() + ITEM_HEIGHT / 2);
  return result;
}


// Index of the near by position in 6 directions
int indexToLeftUp37[] = {
         -1, -1, -1, -1,
       -1,  0,  1,  2,  3,
     -1, 4,  5,  6,  7,  8,
   -1, 9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20,
       22, 23, 24, 25, 26,
         28, 29, 30, 31
};


int ThirtySevenGameBoardInfo::leftUpIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeftUp37[index];
}

int indexToRightUp37[] = {
         -1, -1, -1, -1,
        0,  1,  2,  3, -1,
      4,  5,  6,  7,  8, -1,
    9, 10, 11, 12, 13, 14, -1,
     16, 17, 18, 19, 20, 21,
       23, 24, 25, 26, 27,
         29, 30, 31, 32
};

int ThirtySevenGameBoardInfo::rightUpIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToRightUp37[index];
}


int indexToRight37[] = {
          1,  2,  3, -1,
        5,  6,  7,  8, -1,
     10, 11, 12, 13, 14, -1,
   16, 17, 18, 19, 20, 21, -1,
     23, 24, 25, 26, 27, -1,
       29, 30, 31, 32, -1,
         34, 35, 36, -1
};

int ThirtySevenGameBoardInfo::rightIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToRight37[index];
}

int indexToRightDown37[] = {
          5,  6,  7,  8,
       10, 11, 12, 13, 14,
     16, 17, 18, 19, 20, 21,
   22, 23, 24, 25, 26, 27, -1,
     28, 29, 30, 31, 32, -1,
       33, 34, 35, 36, -1,
         -1, -1, -1, -1,
};

int ThirtySevenGameBoardInfo::rightDownIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToRightDown37[index];
}

int indexToLeftDown37[] = {
          4,  5,  6,  7,
        9, 10, 11, 12, 13,
     15, 16, 17, 18, 19, 20,
   -1, 22, 23, 24, 25, 26, 27,
     -1, 28, 29, 30, 31, 32,
       -1, 33, 34, 35, 36,
         -1, -1, -1, -1
};

int ThirtySevenGameBoardInfo::leftDownIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeftDown37[index];
}

int indexToLeft37[] = {
         -1,  0,  1,  2,
       -1,  4,  5,  6,  7,
     -1,  9, 10, 11, 12, 13,
   -1, 15, 16, 17, 18, 19, 20,
     -1, 22, 23, 24, 25, 26,
       -1, 28, 29, 30, 31,
         -1, 33, 34, 35
};

int ThirtySevenGameBoardInfo::leftIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeft37[index];
}

int ThirtySevenGameBoardInfo::nearbyIndex(int index, int direction)
{
  if (direction < 0 || direction > 5)
    return -1;
  switch (direction)
  {
  case 0:
    return leftDownIndex(index); break;
  case 1:
    return rightDownIndex(index); break;
  case 2:
    return rightIndex(index); break;
  case 3:
    return rightUpIndex(index); break;
  case 4:
    return leftUpIndex(index); break;
  case 5:
    return leftIndex(index); break;
  default:
    return -1;
  }
}

// The chains to reload items(from inner to outter(我这个是不是拼错了-.-))
QVector<QVector<int> > _chains37;
const QVector<QVector<int> >& ThirtySevenGameBoardInfo::chains()
{
  if (_chains37.isEmpty())
  {
    int currentIndexInOriginalChain = 0;
    for (int i = 0;i < CHAIN_NUMBER;++i)
    {
      QVector<int> currentChain;
      for (int j = 0;j < itemCountInAChain37[i];++j)
        currentChain.push_back(originalChain37[currentIndexInOriginalChain++]);
      _chains37.push_back(currentChain);
    }
  }
  return _chains37;
}


// Indexes around the index
// At most 6 indexes
// The order is left, leftUp, rightUp, right, rightDown, leftDown
QVector<QVector<int> > _chainAroundIndex37;
QVector<int> ThirtySevenGameBoardInfo::chainAroundIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return QVector<int>();
  if (_chainAroundIndex37.isEmpty())
    for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
      _chainAroundIndex37.push_back(QVector<int>());
  if (_chainAroundIndex37[index].isEmpty())
  {
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
    _chainAroundIndex37[index] = result;
  }
  return _chainAroundIndex37[index];
}


// Whether the index can be a center to rotate
int indexToCanBeRotateCenter37[] = {
          0,  0,  0,  0,
        0,  1,  1,  1,  0,
      0,  1,  1,  1,  1,  0,
    0,  1,  1,  1,  1,  1,  0,
      0,  1,  1,  1,  1,  0,
        0,  1,  1,  1,  0,
          0,  0,  0,  0,
};

bool ThirtySevenGameBoardInfo::canBeRotateCenter(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return false;
  return indexToCanBeRotateCenter37[index] == 1;
}

// Whether the index is a joint of the chains(circles)
int indexToIsJoint37[] = {
        1,  0,  0,  1,
      0,  1,  0,  1,  0,
    0,  0,  1,  1,  0,  0,
  1,  1,  1,  1,  1,  1,  1,
    0,  0,  1,  1,  0,  0,
      0,  1,  0,  1,  0,
        1,  0,  0,  1,
};
bool ThirtySevenGameBoardInfo::isJoint(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return false;
  return indexToIsJoint37[index];
}

// The index of the item at the position of the scene

int positionToIndex37[] = {
  -1, -1, -1,  0,  0,  1,  1,  2,  2,  3,  3, -1, -1, -1,
  -1, -1,  4,  4,  5,  5,  6,  6,  7,  7,  8,  8, -1, -1,
  -1,  9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, -1,
  15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 20, 21, 21,
  -1, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, -1,
  -1, -1, 28, 28, 29, 29, 30, 30, 31, 31, 32, 32, -1, -1,
  -1, -1, -1, 33, 33, 34, 34, 35, 35, 36, 36, -1, -1, -1
};
int ThirtySevenGameBoardInfo::indexOfPosition(QPointF position)
{
  int c = (position.x() - LOCATION_GAME_BOARD_ITEM_X_FROM) *
          COLUMN_NUMBER /
          (LOCATION_GAME_BOARD_ITEM_X_TO - LOCATION_GAME_BOARD_ITEM_X_FROM);
  int r = (position.y() - LOCATION_GAME_BOARD_ITEM_Y_FROM) *
          ROW_NUMBER /
          (LOCATION_GAME_BOARD_ITEM_Y_TO - LOCATION_GAME_BOARD_ITEM_Y_FROM);
  if (c < 0 || c >= COLUMN_NUMBER || r < 0 || r > ROW_NUMBER)
    return -1;
  int index = positionToIndex37[r * COLUMN_NUMBER + c];
  if (distanceOfTwoPoints(position, centerPositionOfIndex(index)) > ITEM_GESTURE_R)
    return -1;
  return index;
}

// The index of the item at the position of the row and column
int ThirtySevenGameBoardInfo::indexOfPosition(int row, int column)
{
  return positionToIndex37[row * COLUMN_NUMBER + column];
}

// The index of the item at the position of the mouse
int ThirtySevenGameBoardInfo::indexOfMousePosition(QPointF position)
{
  position.setX(position.x() + (ITEM_GESTURE_R - ITEM_WIDTH) / 2);
  position.setY(position.y() + (ITEM_GESTURE_R - ITEM_HEIGHT) / 2);
  return indexOfPosition(position);
}

// Index of the first/last position in 6 directions
int rowToFirstIndex37[] = {
  0,  4, 9, 15, 22, 28, 33
};

int rowToLastIndex37[] = {
  3,  8, 14, 21, 27, 32, 36
};

int ThirtySevenGameBoardInfo::firstOfRow(int row)
{
  if (row < 0 && row >= ROW_NUMBER)
    return -1;
  return rowToFirstIndex37[row];
}

int ThirtySevenGameBoardInfo::lastOfRow(int row)
{
  if (row < 0 && row >= ROW_NUMBER)
    return -1;
  return rowToLastIndex37[row];
}

int indexToFirstLeftUpIndex37[] = {
          0,  1,  2,  3,
        4,  0,  1,  2,  3,
      9,  4,  0,  1,  2,  3,
   15,  9,  4,  0,  1,  2,  3,
     15,  9,  4,  0,  1,  2,
       15,  9,  4,  0,  1,
         15,  9,  4,  0,
};

int ThirtySevenGameBoardInfo::firstOfLeftUp(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToFirstLeftUpIndex37[index];
}

int indexToLastLeftUpIndex37[] = {
         36, 32, 27, 21,
       35, 36, 32, 27, 21,
     34, 35, 36, 32, 27, 21,
   33, 34, 35, 36, 32, 27, 21,
     33, 34, 35, 36, 32, 27,
       33, 34, 35, 36, 32,
         33, 34, 35, 36
};

int indexToFirstLeftDownIndex37[] = {
         15, 22, 28, 33,
       15, 22, 28, 33, 34,
     15, 22, 28, 33, 34, 35,
   15, 22, 28, 33, 34, 35, 36,
     22, 28, 33, 34, 35, 36,
       28, 33, 34, 35, 36,
         33, 34, 35, 36
};

int ThirtySevenGameBoardInfo::firstOfLeftDown(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToFirstLeftDownIndex37[index];
}

int ThirtySevenGameBoardInfo::lastOfLeftUp(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLastLeftUpIndex37[index];
}

int indexToLastLeftDownIndex37[] = {
          0,  1,  2,  3,
        0,  1,  2,  3,  8,
      0,  1,  2,  3,  8, 14,
    0,  1,  2,  3,  8, 14, 21,
      1,  2,  3,  8, 14, 21,
        2,  3,  8, 14, 21,
          3,  8, 14, 21
};

int ThirtySevenGameBoardInfo::lastOfLeftDown(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLastLeftDownIndex37[index];
}

// Inverval between two layers
double ThirtySevenGameBoardInfo::intervalBetweenTwoLayers()
{
  return LOCATION_GAME_BOARD_ITEM_X_INTERVAL;
}

// Center position of the gameboard
QPointF ThirtySevenGameBoardInfo::centerPos()
{
  return positionOfIndex(originalChain37[0]);
}
