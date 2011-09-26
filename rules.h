/*
** Classes of rules may be used in a game, nothing needed to be explained here.
*/

#ifndef RULES_H
#define RULES_H

#include "abstractrule.h"

/**
 * @brief A class of a rule of the swap classic game.
 */
class SwapClassicGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the rotate classic game.
 */
class RotateClassicGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the swap endless game.
 */
class SwapEndlessGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the rotate endless game.
 */
class RotateEndlessGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the swap timing game.
 */
class SwapTimingGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the rotate timing game.
 */
class RotateTimingGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the puzzle game.
 */
class PuzzleGameRule : public AbstractRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture)=0;
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep);
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination);
  virtual bool endlessFill();
};

/**
 * @brief A class of a rule of the rotate puzzle game.
 */
class RotatePuzzleGameRule : public PuzzleGameRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
};

/**
 * @brief A class of a rule of the swap puzzle game(not used).
 */
class SwapPuzzleGameRule : public PuzzleGameRule
{
public:
  virtual bool gestureAllowed(AbstractRule::Gesture gesture);
};

#endif // RULES_H
