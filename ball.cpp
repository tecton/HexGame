#include "ball.h"

Ball::Ball(Ball::Color theColor) :
           color(theColor),
           state(JustCreated),
           locked(false)
{}

bool Ball::advance()
{
  Ball::State lastState = state;

  // If there is a position to stop
  if (!stopPositions.empty())
  {
    // Change the state
    state = SystemMoving;

    // Change the position
    position = stopPositions[stopPositions.size() - 1];

    // Delete the position in the vector
    stopPositions.pop_back();

    // Currently not used, may be used later to speed up
    if (stopPositions.size() < 2)
      state = AlmostStable;

    // Change the state
    if (stopPositions.size() == 0)
      state = Stable;
  }

  return state != lastState;
}
