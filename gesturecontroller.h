/*
** Class of a controller which controls the gesture and let the core controller to do things
*/

#ifndef GESTURECONTROLLER_H
#define GESTURECONTROLLER_H

// File must include
#include <QVector>
#include "abstractrule.h"

// Forward declaration
class QPointF;
class AbstractRule;
class AbstractGameBoardInfo;
class CoreController;
class EffectPainter;

class GestureController
{
public:
  // State of the gesture
  enum GestureState {NoGesture, ChooseGesture, LocateGesture};

  // Constructor with the rule, the infomation of the gameboard,
  // the core controller, the effect painter
  GestureController(AbstractRule *theRule,
                    AbstractGameBoardInfo *theGameBoardInfo,
                    CoreController *theController,
                    EffectPainter *theEffectPainter);

  // 3 functions to deal with the mouse events
  // Press
  //   maintain:
  //     gestureState,
  //     gestureIndexes
  void dealPressed(const QPointF& pos);
  // Move
  //   if haven't confirm which gesture it is, append _gesture_indexes
  //   if the gesture is confirmed, set the new positions of each balls
  //   influenced
  void dealMoved(const QPointF& pos);
  // Release
  //   put balls influenced to correct position
  //   exam whether it can be put there
  //   if can then do something
  //   if not then rollback
  void dealReleased(const QPointF& pos);

private:
  // Rule of the game
  AbstractRule *rule;

  // Infomation of the gameboard
  AbstractGameBoardInfo *gameboardInfo;

  // Core controller which controls the balls
  CoreController *controller;

  // The effective painter
  EffectPainter *effectPainter;

  // The gesture allowed
  AbstractRule::Gesture gesture;

  // State of the gesture
  GestureState gestureState;

  // Indexes of the gesture
  QVector <int> gestureIndexes;

  // Test the gesture
  void testGesture(const QPointF& pos);
};

#endif // GESTURECONTROLLER_H
