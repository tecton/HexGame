/*
** Abstract class of a gameboard.
** It should return quite a lot of infomation about location.
*/

#ifndef ABSTRACTGAMEBOARDINFO_H
#define ABSTRACTGAMEBOARDINFO_H

// File must include
#include <QVector>
#include <QPointF>

class AbstractGameBoardInfo
{
public:
  // Width of the gameboard
  virtual int width()=0;

  // Height of the gameboard
  virtual int height()=0;

  // Radius of the ball
  virtual double ballR()=0;

  // Number of the balls
  virtual int totalBallCounts()=0;

  // Number of chains
  virtual int chainNumber()=0;

  // The column number of the item with the index
  virtual int columnOfIndex(int index)=0;

  // The row number of the item with the index
  virtual int rowOfIndex(int index)=0;

  // Left Top of the item with the index
  virtual QPointF positionOfIndex(int index)=0;

  // Center of the item with the index
  virtual QPointF centerPositionOfIndex(int index)=0;

  // Index of the near by position in 6 directions
  virtual int leftIndex(int index)=0;
  virtual int leftUpIndex(int index)=0;
  virtual int rightUpIndex(int index)=0;
  virtual int rightIndex(int index)=0;
  virtual int rightDownIndex(int index)=0;
  virtual int leftDownIndex(int index)=0;

  // Index of the near by position
  virtual int nearbyIndex(int index, int direction)=0;

  // The chains to reload items(from inner to outter)
  virtual const QVector<QVector<int> >& chains()=0;

  // Indexes around the index
  // At most 6 indexes
  // The order is left, leftUp, rightUp, right, rightDown, leftDown
  virtual QVector<int> chainAroundIndex(int index)=0;

  // Whether the index can be a center to rotate
  virtual bool canBeRotateCenter(int index)=0;

  // Whether the index is a joint of the chains(circles)
  virtual bool isJoint(int index)=0;

  // The index of the item at the position of the scene
  virtual int indexOfPosition(QPointF position)=0;

  // The index of the item at the position of the row and column
  virtual int indexOfPosition(int row, int column)=0;

  // The index of the item at the position of the mouse
  virtual int indexOfMousePosition(QPointF position)=0;

  // Index of the first/last position in 6 directions
  virtual int firstOfRow(int row)=0;
  virtual int lastOfRow(int row)=0;
  virtual int firstOfLeftUp(int index)=0;
  virtual int lastOfLeftUp(int index)=0;
  virtual int firstOfLeftDown(int index)=0;
  virtual int lastOfLeftDown(int index)=0;

  // Inverval between two layers
  virtual double intervalBetweenTwoLayers()=0;

  // Center position of the gameboard
  virtual QPointF centerPos()=0;
};

#endif // ABSTRACTGAMEBOARDINFO_H
