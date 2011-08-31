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

bool SwapEndlessGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Swap:
    return true;
  default:
    return false;
  }
}

bool SwapEndlessGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
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


bool SwapEndlessGameRule::eliminationAllowed
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

bool SwapEndlessGameRule::endlessFill()
{
  return true;
}

//////////////////////////////////////////////////////////////////////////

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

bool SwapTimingGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Swap:
    return true;
  default:
    return false;
  }
}

bool SwapTimingGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
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


bool SwapTimingGameRule::eliminationAllowed
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

bool SwapTimingGameRule::endlessFill()
{
  return true;
}

////////////////////////////////////////////////////////////////////////

bool RotateTimingGameRule::gestureAllowed(AbstractRule::Gesture gesture)
{
  switch (gesture)
  {
  case Rotate:
    return true;
  default:
    return false;
  }
}

bool RotateTimingGameRule::gameStepAllowed(AbstractRule::GameStep gameStep)
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


bool RotateTimingGameRule::eliminationAllowed
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

bool RotateTimingGameRule::endlessFill()
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
