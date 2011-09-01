#include "sixtyonegameboardinfo.h"

#include <qmath.h>
#include "gamemath.h"

// The width of the item
#define ITEM_WIDTH                              45

// The height of the item
#define ITEM_HEIGHT                             45

// The width of the item in a gesture
#define ITEM_GESTURE_R                          35

// The absolute position of game(I must draw a picture~~~)
#define LOCATION_GAME_VIEW_X_FROM               0
#define LOCATION_GAME_VIEW_Y_FROM               0
#define LOCATION_GAME_VIEW_X_TO                 1024
#define LOCATION_GAME_VIEW_Y_TO                 600

#define LOCATION_GAME_BOARD_ITEM_X_FROM         370
#define LOCATION_GAME_BOARD_ITEM_Y_FROM         54
#define LOCATION_GAME_BOARD_ITEM_X_TO           961
#define LOCATION_GAME_BOARD_ITEM_Y_TO           569

#define LOCATION_GAME_BOARD_ITEM_X_INTERVAL     (LOCATION_GAME_BOARD_ITEM_X_TO - \
                                                 LOCATION_GAME_BOARD_ITEM_X_FROM) / \
                                                 COLUMN_NUMBER
#define LOCATION_GAME_BOARD_ITEM_Y_INTERVAL     (LOCATION_GAME_BOARD_ITEM_Y_TO - \
                                                 LOCATION_GAME_BOARD_ITEM_Y_FROM) / \
                                                 ROW_NUMBER

// Total item number
#define TOTAL_ITEM_NUMBER                       61
// Total row number
#define ROW_NUMBER                              9
// Total column number
#define COLUMN_NUMBER                           18
// Total chain(circle) number
#define CHAIN_NUMBER                            5

// The offset of the scene and mouse
#define SCENE_TO_MOUSE_DX                       (ITEM_WIDTH / 2)
#define SCENE_TO_MOUSE_DY                       (ITEM_HEIGHT / 2)

// Width of the gameboard
int SixtyOneGameBoardInfo::width()
{
  return LOCATION_GAME_VIEW_X_TO - LOCATION_GAME_VIEW_X_FROM;
}

// Height of the gameboard
int SixtyOneGameBoardInfo::height()
{
  return LOCATION_GAME_VIEW_Y_TO - LOCATION_GAME_VIEW_Y_FROM;
}

// Radius of the ball
double SixtyOneGameBoardInfo::ballR()
{
  return ITEM_WIDTH / 2;
}

// Number of the balls
int SixtyOneGameBoardInfo::totalBallCounts()
{
  return TOTAL_ITEM_NUMBER;
}

int SixtyOneGameBoardInfo::chainNumber()
{
  return CHAIN_NUMBER;
}

int col[] = {
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

// The column number of the item with the index
int SixtyOneGameBoardInfo::columnOfIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return 0;
  return col[index];
}


int row[] = {
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


// The row number of the item with the index

int SixtyOneGameBoardInfo::rowOfIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return 0;
  return row[index];
}

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


// Left Top of the item with the index

QVector<QPointF> _positionOfIndex;
QPointF SixtyOneGameBoardInfo::positionOfIndex(int index)
{
  if (_positionOfIndex.isEmpty())
  {
    for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
    {
      int r = rowOfIndex(i);
      int c = columnOfIndex(i);

      qreal x = LOCATION_GAME_BOARD_ITEM_X_FROM +
                c * LOCATION_GAME_BOARD_ITEM_X_INTERVAL;
      qreal y = LOCATION_GAME_BOARD_ITEM_Y_FROM +
                r * LOCATION_GAME_BOARD_ITEM_Y_INTERVAL;
      _positionOfIndex.push_back(QPointF(x, y));
    }

  }
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return QPointF(LOCATION_GAME_BOARD_ITEM_X_FROM,
                   LOCATION_GAME_BOARD_ITEM_Y_FROM);
  return _positionOfIndex[index];
}

// Center of the item with the index
QPointF SixtyOneGameBoardInfo::centerPositionOfIndex(int index)
{
  QPointF result = positionOfIndex(index);
  result.setX(result.x() + ITEM_WIDTH / 2);
  result.setY(result.y() + ITEM_HEIGHT / 2);
  return result;
}




// Index of the near by position in 6 directions
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

int SixtyOneGameBoardInfo::leftUpIndex(int index)
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

int SixtyOneGameBoardInfo::rightUpIndex(int index)
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
           57, 58, 59, 60, -1
};

int SixtyOneGameBoardInfo::rightIndex(int index)
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

int SixtyOneGameBoardInfo::rightDownIndex(int index)
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

int SixtyOneGameBoardInfo::leftDownIndex(int index)
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

int SixtyOneGameBoardInfo::leftIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLeft[index];
}

int SixtyOneGameBoardInfo::nearbyIndex(int index, int direction)
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
QVector<QVector<int> > _chains;
const QVector<QVector<int> >& SixtyOneGameBoardInfo::chains()
{
  if (_chains.isEmpty())
  {
    int currentIndexInOriginalChain = 0;
    for (int i = 0;i < CHAIN_NUMBER;++i)
    {
      QVector<int> currentChain;
      for (int j = 0;j < itemCountInAChain[i];++j)
        currentChain.push_back(originalChain[currentIndexInOriginalChain++]);
      _chains.push_back(currentChain);
    }
  }
  return _chains;
}


// Indexes around the index
// At most 6 indexes
// The order is left, leftUp, rightUp, right, rightDown, leftDown
QVector<QVector<int> > _chainAroundIndex;
QVector<int> SixtyOneGameBoardInfo::chainAroundIndex(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return QVector<int>();
  if (_chainAroundIndex.isEmpty())
    for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
      _chainAroundIndex.push_back(QVector<int>());
  if (_chainAroundIndex[index].isEmpty())
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
    _chainAroundIndex[index] = result;
  }
  return _chainAroundIndex[index];
}


// Whether the index can be a center to rotate
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

bool SixtyOneGameBoardInfo::canBeRotateCenter(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return false;
  return indexToCanBeRotateCenter[index] == 1;
}

// Whether the index is a joint of the chains(circles)
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
bool SixtyOneGameBoardInfo::isJoint(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return false;
  return indexToIsJoint[index];
}

// The index of the item at the position of the scene
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
int SixtyOneGameBoardInfo::indexOfPosition(QPointF position)
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
  if (distanceOfTwoPoints(position, centerPositionOfIndex(index)) > ITEM_GESTURE_R)
    return -1;
  return index;
}

// The index of the item at the position of the row and column
int SixtyOneGameBoardInfo::indexOfPosition(int row, int column)
{
  return positionToIndex[row * COLUMN_NUMBER + column];
}

// The index of the item at the position of the mouse
int SixtyOneGameBoardInfo::indexOfMousePosition(QPointF position)
{
  position.setX(position.x() + (ITEM_GESTURE_R - ITEM_WIDTH) / 2);
  position.setY(position.y() + (ITEM_GESTURE_R - ITEM_HEIGHT) / 2);
  return indexOfPosition(position);
}

// Index of the first/last position in 6 directions
int rowToFirstIndex[] = {
  0,  5, 11, 18, 26, 35, 43, 50, 56
};

int rowToLastIndex[] = {
  4, 10, 17, 25, 34, 42, 49, 55, 60
};

int SixtyOneGameBoardInfo::firstOfRow(int row)
{
  if (row < 0 && row >= ROW_NUMBER)
    return -1;
  return rowToFirstIndex[row];
}

int SixtyOneGameBoardInfo::lastOfRow(int row)
{
  if (row < 0 && row >= ROW_NUMBER)
    return -1;
  return rowToLastIndex[row];
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

int SixtyOneGameBoardInfo::firstOfLeftUp(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToFirstLeftUpIndex[index];
}

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

int SixtyOneGameBoardInfo::firstOfLeftDown(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToFirstLeftDownIndex[index];
}

int SixtyOneGameBoardInfo::lastOfLeftUp(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLastLeftUpIndex[index];
}

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

int SixtyOneGameBoardInfo::lastOfLeftDown(int index)
{
  if (index < 0 || index >= TOTAL_ITEM_NUMBER)
    return -1;
  return indexToLastLeftDownIndex[index];
}

// Inverval between two layers
double SixtyOneGameBoardInfo::intervalBetweenTwoLayers()
{
  return LOCATION_GAME_BOARD_ITEM_X_INTERVAL;
}

// Center position of the gameboard
QPointF SixtyOneGameBoardInfo::centerPos()
{
  return positionOfIndex(30);
}
