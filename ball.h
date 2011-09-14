/*
** Class of a ball
*/

#ifndef BALL_H
#define BALL_H

// File must include
#include <QVector>
#include <QPointF>

// Forward declaration
class CoreController;

class Ball
{
  friend class CoreController;
public:
  // Item colors
  // (must start from 0)
  enum Color{Red = 0, Blue, Green, Yellow, Purple, White,
             Orange, Brown, BadColor};

  // Item states
  //   STABLE:
  //     Item is on a stable position.
  //   USER_MOVING:
  //     Item is moved by the user.
  //   SYSTEM_MOVING:
  //     Item is moved by the system.
  //   JUST_CREATED:
  //     Item has just been created wating for additional
  //     operations.
  enum State{Stable, AlmostStable, UserMoving,
             UserReleased, SystemMoving, JustCreated};

  // Constructor with the color
  Ball(Ball::Color theColor = Ball::Red);

  // Set the color
  inline void setColor(Ball::Color theColor)
  {color = theColor;}

  // Get the color
  inline Ball::Color getColor()
  {return color;}

  // Set the state
  inline void setState(Ball::State theState)
  {state = theState;}

  // Get the state
  inline Ball::State getState()
  {return state;}

  // Set whether it's locked
  inline void setLocked(bool isLocked)
  {locked = isLocked;}

  // Get whether it's locked
  inline bool getLocked()
  {return locked;}

  // Set the position
  inline void setPos(QPointF pos)
  {position = pos;}

  // Get the position
  inline QPointF pos()
  {return position;}

  // Advance the ball, may change the position and state
  void advance();

  // Whether two balls have same color
  bool sameColor(Ball* anotherBall)
  {return color != BadColor &&
          anotherBall &&
          color == anotherBall->color;}

private:
  // Color of the ball
  Color color;

  // State of the ball
  State state;

  // Whether the ball is locked
  bool locked;

  // Current position of the ball
  QPointF position;

  // Positions the ball should be later
  QVector<QPointF> stopPositions;
};

#endif // BALL_H
