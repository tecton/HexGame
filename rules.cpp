#include "rules.h"


bool SwapClassicGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Swap:
    return true;
  default:
    return false;
  }
}

bool SwapClassicGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
{
  switch (gameStep)
  {
  case FillWithNewBalls:
  case AutoRotate:
  case RemindElimination:
  case Eliminate:
  case RollBackWhenNoElimination:
    return true;
  default:
    return false;
  }
}


bool SwapClassicGameRule::eliminationAllowed
    (AbstractRule::Elimination elimination)
{
  switch (elimination)
  {
  case ThreeInARow:
    return true;
  default:
    return false;
  }
}

bool SwapClassicGameRule::endlessFill()
{
  return false;
}

//////////////////////////////////////////////////////////////////////////

bool RotateClassicGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Rotate:
    return true;
  default:
    return false;
  }
}

bool RotateClassicGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
{
  switch (gameStep)
  {
  case FillWithNewBalls:
  case AutoRotate:
  case RemindElimination:
  case Eliminate:
  case RollBackWhenNoElimination:
    return true;
  default:
    return false;
  }
}

bool RotateClassicGameRule::eliminationAllowed
    (AbstractRule::Elimination elimination)
{
  switch (elimination)
  {
  case ThreeInARow:
  case Circle:
    return true;
  default:
    return false;
  }
}

bool RotateClassicGameRule::endlessFill()
{
  return false;
}

///////////////////////////////////////////////////////////////////////////

bool RotateEndlessGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Rotate:
    return true;
  default:
    return false;
  }
}

bool RotateEndlessGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
{
  switch (gameStep)
  {
  case FillWithNewBalls:
  case AutoRotate:
  case RemindElimination:
  case Eliminate:
  case RollBackWhenNoElimination:
    return true;
  default:
    return false;
  }
}


bool RotateEndlessGameRule::eliminationAllowed
    (AbstractRule::Elimination elimination)
{
  switch (elimination)
  {
  case ThreeInARow:
  case Circle:
    return true;
  default:
    return false;
  }
}

bool RotateEndlessGameRule::endlessFill()
{
  return true;
}

////////////////////////////////////////////////////////////////////////

bool RotateTimeGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Rotate:
    return true;
  default:
    return false;
  }
}

bool RotateTimeGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
{
  switch (gameStep)
  {
  case FillWithNewBalls:
  case AutoRotate:
  case RemindElimination:
  case Eliminate:
  case RollBackWhenNoElimination:
    return true;
  default:
    return false;
  }
}


bool RotateTimeGameRule::eliminationAllowed
    (AbstractRule::Elimination elimination)
{
  switch (elimination)
  {
  case ThreeInARow:
  case Circle:
    return true;
  default:
    return false;
  }
}

bool RotateTimeGameRule::endlessFill()
{
  return true;
}

////////////////////////////////////////////////////////////////////////

bool PuzzleGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
{
  switch (gameStep)
  {
  default:
    return false;
  }
}


bool PuzzleGameRule::eliminationAllowed
    (AbstractRule::Elimination elimination)
{
  switch (elimination)
  {
  default:
    return false;
  }
}

bool PuzzleGameRule::endlessFill()
{
  return true;
}

////////////////////////////////////////////////////////////////////////

bool RotatePuzzleGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Rotate:
    return true;
  default:
    return false;
  }
}

////////////////////////////////////////////////////////////////////////

bool SwapPuzzleGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Swap:
    return true;
  default:
    return false;
  }
}