#include "ball.h"


Ball::Ball(Ball::Color theColor) :
           color(theColor),
           state(JustCreated)
{}


void Ball::advance()
{
  if (!stopPositions.empty())
  {
    state = SystemMoving;
    position = stopPositions[stopPositions.size() - 1];
    stopPositions.pop_back();
      if (stopPositions.size() < 4)
        state = AlmostStable;
    if (stopPositions.size() == 0)
      state = Stable;
  }
}
