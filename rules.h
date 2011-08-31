#ifndef RULES_H
#define RULES_H

#include "abstractrule.h"

class SwapClassicGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class RotateClassicGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class SwapEndlessGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class RotateEndlessGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class SwapTimingGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class RotateTimingGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class PuzzleGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture)=0;
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

class RotatePuzzleGameRule : public PuzzleGameRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
};

class SwapPuzzleGameRule : public PuzzleGameRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
};
#endif // RULES_H
