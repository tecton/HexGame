#ifndef BALL_H
#define BALL_H

// File must include
#include <QVector>
#include <QPointF>

// Forward declaration
class CoreController;

/**
 * @brief A class of a ball.
 */
class Ball
{
  friend class CoreController;
public:
  /**
   * @brief Color of the ball.
   *
   * Currently there are 8 colors.
   * @warning Orange and Brown don't have the correct pixmap.
   */
  enum Color{Red = 0, Blue, Green, Yellow, Purple, White,
             Orange, Brown, BadColor};

  /**
   * @brief State of the ball.
   *
   * Currently there are 6 states.
   *   STABLE:
   *     Item is on a stable position.
   *   AlmostStable:
   *     Item will be on a stable position soon.
   *   USER_MOVING:
   *     Item is moved by the user.
   *   UserReleased:
   *     Item is released by the user.
   *   SYSTEM_MOVING:
   *     Item is moved by the system.
   *   JUST_CREATED:
   *     Item has just been created wating for additional operations.
   * @warning AlmostStable aren't used, but you can use it if you turn a
   * comment in ball.cpp be code.
   */
  enum State{Stable, AlmostStable, UserMoving,
             UserReleased, SystemMoving, JustCreated};

  /**
   * @brief Constructor with the color.
   */
  Ball(Ball::Color theColor = Ball::Red);

  /**
   *@brief Set the color of the ball.
   */
  inline void setColor(Ball::Color theColor)
  {color = theColor;}

  /**
   *@brief Get the color of the ball.
   */
  inline Ball::Color getColor()
  {return color;}

  /**
   *@brief Set the state of the ball.
   */
  inline void setState(Ball::State theState)
  {state = theState;}

  /**
   *@brief Get the state of the ball.
   */
  inline Ball::State getState()
  {return state;}

  /**
   *@brief Set whether the ball is locked.
   */
  inline void setLocked(bool isLocked)
  {locked = isLocked;}

  /**
   *@brief Get whether the ball is locked.
   */
  inline bool getLocked()
  {return locked;}

  /**
   *@brief Set the position of the ball.
   */
  inline void setPos(QPointF pos)
  {position = pos;}

  /**
   *@brief Get the position of the ball.
   */
  inline QPointF pos()
  {return position;}

  /**
   *@brief Advance the ball, may change the position and state.
   */
  void advance();

  /**
   *@brief Whether two balls have the same color.
   *
   *@param anotherBall Pointer of another ball.
   *@return Whether two balls have the same color.
   */
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
