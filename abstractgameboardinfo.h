#ifndef ABSTRACTGAMEBOARDINFO_H
#define ABSTRACTGAMEBOARDINFO_H

// File must include
#include <QVector>
#include <QPointF>


/**
 * @brief An abstract class of a gameboard.
 * It should return quite a lot of infomation about location.
 */
class AbstractGameBoardInfo
{
public:
  /**
   *@brief Width of the gameboard.
   *
   *@return Width of the gameboard.
   */
  virtual int width()=0;

  /**
   *@brief Height of the gameboard.
   *
   *@return Height of the gameboard.
   */
  virtual int height()=0;

  /**
   *@brief Radius of the ball.
   *
   *@return Radius of the ball.
   */
  virtual double ballR()=0;

  /**
   *@brief Number of the balls.
   *
   *@return Number of the balls.
   */
  virtual int totalBallCounts()=0;

  /**
   *@brief Number of the chains.
   *
   *@return Number of the chains.
   */
  virtual int chainNumber()=0;

  /**
   *@brief The column number of the ball with the index.
   *
   * A ball occupies 2 columns.
   *@return The column number of the ball with the index.
   */
  virtual int columnOfIndex(int index)=0;

  /**
   *@brief The row number of the ball with the index.
   *
   *@return The row number of the ball with the index.
   */
  virtual int rowOfIndex(int index)=0;

  /**
   *@brief The center position of the ball with the index.
   *
   *@return The center position of the ball with the index.
   */
  virtual QPointF positionOfIndex(int index)=0;

  /**
   *@brief Index of the left ball.
   */
  virtual int leftIndex(int index)=0;

  /**
   *@brief Index of the left up ball.
   */
  virtual int leftUpIndex(int index)=0;

  /**
   *@brief Index of the right up ball.
   */
  virtual int rightUpIndex(int index)=0;

  /**
   *@brief Index of the right ball.
   */
  virtual int rightIndex(int index)=0;

  /**
   *@brief Index of the right down ball.
   */
  virtual int rightDownIndex(int index)=0;

  /**
   *@brief Index of the left down ball.
   */
  virtual int leftDownIndex(int index)=0;

  /**
   *@brief Index of the near by position with the direction.
   *
   * 0 : Left down
   * 1 : Right down
   * 2 : Right
   * 3 : Right up
   * 4 : Left up
   * 5 : Left
   * It's mainly used to calculate the connections, so the order of the
   * directions is different from other functions.
   *@return Index of the near by position with the direction.
   */
  virtual int nearbyIndex(int index, int direction)=0;

  /**
   *@brief The chains to reload balls(from inner to outter).
   *
   *@return The chains to reload balls(from inner to outter).
   */
  virtual const QVector<QVector<int> >& chains()=0;

  /**
   *@brief Indexes around the index.
   *
   * The order is left, left up, right up, right, right down, left down
   *@return Indexes around the index.
   */
  virtual QVector<int> chainAroundIndex(int index)=0;

  /**
   *@brief Whether the index can be a center to rotate.
   *
   * In a simple way to explain, whether it's not on the border.
   * Of course you can make some of the ball not on the border not able to
   * be a center to rotate according to your needs.
   *@return Whether the index can be a center to rotate.
   */
  virtual bool canBeRotateCenter(int index)=0;

  /**
   *@brief Whether the index is a joint of the chains(in a circle).
   *
   * A chain has 6 borders.
   * When we do the animation to rotate the balls when filling the gameboard,
   * this function helps to speed up it.
   *@return Whether the index is a joint of the chains(in a circle).
   */
  virtual bool isJoint(int index)=0;

  /**
   *@brief The index of the ball at the position.
   *
   * It is used to locate a ball according to the user's gesture.
   *@return The index of the ball at the position.
   */
  virtual int indexOfPosition(QPointF position)=0;

  /**
   *@brief The index of the ball at the position of the row and column.
   *
   * Will be used by the "indexOfPosition(QPointF position)" function.
   *@return The index of the ball at the position of the row and column.
   */
  virtual int indexOfPosition(int row, int column)=0;

  /**
   *@brief Index of the first one in left -- right direction.
   */
  virtual int firstOfRow(int row)=0;

  /**
   *@brief Index of the last one in left -- right direction.
   */
  virtual int lastOfRow(int row)=0;

  /**
   *@brief Index of the first one in left up -- right down direction.
   */
  virtual int firstOfLeftUp(int index)=0;

  /**
   *@brief Index of the last one in left up -- right down direction.
   */
  virtual int lastOfLeftUp(int index)=0;

  /**
   *@brief Index of the first one in left down -- right up direction.
   */
  virtual int firstOfLeftDown(int index)=0;

  /**
   *@brief Index of the last one in left down -- right up direction.
   */
  virtual int lastOfLeftDown(int index)=0;

  /**
   *@brief Index of the first in the chain.
   *
   * It is used to know the position the ball is created.
   *@return Index of the first in the chain.
   */
  virtual int firstOfChain(int index)=0;

  /**
   *@brief Inverval between two layers in X direction.
   *
   *@return Inverval between two layers in X direction.
   */
  virtual double intervalBetweenTwoLayers()=0;

  /**
   *@brief Center position of the gameboard.
   *
   * It is used to know the center of the balls.
   *@return Center position of the gameboard.
   */
  virtual QPointF centerPos()=0;
};

#endif // ABSTRACTGAMEBOARDINFO_H
