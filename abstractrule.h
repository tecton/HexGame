#ifndef ABSTRACTRULE_H
#define ABSTRACTRULE_H

/**
 * @brief An abstract class of a rule of the game.
 */
class AbstractRule
{
public:
  /**
   * @brief Gestures
   *
   * Currently only 2 gestures supported(The slide gesture was abandoned).
   */
  enum Gesture{Swap=0/*, Slide*/, Rotate, BadGesture};

  /**
   * @brief GameSteps
   *
   * Currently there are 5 steps of the game.
   */
  enum GameStep{FillWithNewBalls=0,
                AutoRotate,
                RemindElimination,
                Eliminate,
                RollBackWhenNoElimination};

  /**
   * @brief Eliminations
   *
   * Currently there are 2 elimination allowed.
   */
  enum Elimination{ThreeInARow=0, Circle};

  /**
   *@brief Whether a gesture is allowed.
   *
   *@param gesture The gesture.
   *@return Whether a gesture is allowed.
   */
  virtual bool gestureAllowed(AbstractRule::Gesture gesture)=0;

  /**
   *@brief Whether a step is allowed.
   *
   *@param gameStep The step.
   *@return Whether a step is allowed.
   */
  virtual bool gameStepAllowed(AbstractRule::GameStep gameStep)=0;

  /**
   *@brief Whether an elimination is allowed.
   *
   *@param elimination The elimination.
   *@return Whether an elimination is allowed.
   */
  virtual bool eliminationAllowed(AbstractRule::Elimination elimination)=0;

  /**
   *@brief Whether to keep the user not to died.
   *
   *@return Whether to keep the user not to died.
   */
  //
  virtual bool endlessFill()=0;
};

#endif // ABSTRACTRULE_H
