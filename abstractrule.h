/*
** Abstract class of a rule of the game.
*/

#ifndef ABSTRACTRULE_H
#define ABSTRACTRULE_H

class AbstractRule
{
public:
  // Gestures
  enum Gesture{Swap=0/*, Slide*/, Rotate, BadGesture};

  // GameSteps
  enum GameStep{FillWithNewBalls=0,
                AutoRotate,
                RemindElimination,
                Eliminate,
                RollBackWhenNoElimination};

  // Eliminations
  enum Elimination{ThreeInARow=0, Circle};

  // Whether a gesture is allowed
  virtual bool gestureAllowed
      (AbstractRule::Gesture gesture)=0;

  // Whether a step is allowed
  virtual bool gameStepAllowed
      (AbstractRule::GameStep gameStep)=0;

  // Whether an elimination is allowed
  virtual bool eliminationAllowed
      (AbstractRule::Elimination elimination)=0;

  // Whether to keep the user not to died
  virtual bool endlessFill()=0;
};

#endif // ABSTRACTRULE_H
