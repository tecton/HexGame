#ifndef THIRTYSEVENGAMEBOARDINFO_H
#define THIRTYSEVENGAMEBOARDINFO_H

#include "abstractgameboardinfo.h"

#include <QVector>
#include <QPointF>

class ThirtySevenGameBoardInfo : public AbstractGameBoardInfo
{

public:
  // Width of the gameboard
  virtual int width();

  // Height of the gameboard
  virtual int height();

  // Radius of the ball
  virtual double ballR();

  // Number of the balls
  virtual int totalBallCounts();

  // Number of chains
  virtual int chainNumber();

  // The column number of the item with the index
  virtual int columnOfIndex(int index);

  // The row number of the item with the index
  virtual int rowOfIndex(int index);

  // Left Top of the item with the index
  virtual QPointF positionOfIndex(int index);

  // Center of the item with the index
  virtual QPointF centerPositionOfIndex(int index);

  // Index of the near by position in 6 directions
  virtual int leftIndex(int index);
  virtual int leftUpIndex(int index);
  virtual int rightUpIndex(int index);
  virtual int rightIndex(int index);
  virtual int rightDownIndex(int index);
  virtual int leftDownIndex(int index);

  // Index of the near by position
  virtual int nearbyIndex(int index, int direction);

  // The chains to reload items(from inner to outter
  virtual const QVector<QVector<int> >& chains();

  // Indexes around the index
  // At most 6 indexes
  // The order is left, leftUp, rightUp, right, rightDown, leftDown
  virtual QVector<int> chainAroundIndex(int index);

  // Whether the index can be a center to rotate
  virtual bool canBeRotateCenter(int index);

  // Whether the index is a joint of the chains(circles)
  virtual bool isJoint(int index);

  // The index of the item at the position of the scene
  virtual int indexOfPosition(QPointF position);

  // The index of the item at the position of the row and column
  virtual int indexOfPosition(int row, int column);

  // The index of the item at the position of the mouse
  virtual int indexOfMousePosition(QPointF position);

  // Index of the first/last position in 6 directions
  virtual int firstOfRow(int row);
  virtual int lastOfRow(int row);
  virtual int firstOfLeftUp(int index);
  virtual int lastOfLeftUp(int index);
  virtual int firstOfLeftDown(int index);
  virtual int lastOfLeftDown(int index);

  // Index of the first in the chain
  virtual int firstOfChain(int index);

  // Inverval between two layers
  virtual double intervalBetweenTwoLayers();

  // Center position of the gameboard
  virtual QPointF centerPos();
};

#endif // THIRTYSEVENGAMEBOARDINFO_H
