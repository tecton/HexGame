#ifndef GESTURECONTROLLER_H
#define GESTURECONTROLLER_H

#include <QVector>
#include "abstractrule.h"

class QPointF;
class AbstractRule;
class AbstractGameBoardInfo;
class CoreController;

class GestureController
{
public:
  enum GestureState {NoGesture, ChooseGesture, LocateGesture};
  GestureController(AbstractRule *theRule,
                    AbstractGameBoardInfo *theGameBoardInfo,
                    CoreController *theController);

  // 3 functions to deal with the mouse events
  // Press
  //   maintain:
  //     _gesture_state,
  //     _gesture_indexes
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
  AbstractRule *rule;
  AbstractGameBoardInfo *gameboardInfo;
  CoreController *controller;

  AbstractRule::Gesture gesture;
  GestureState gestureState;
  QVector <int> gestureIndexes;

  void testGesture(const QPointF& pos);

};

#endif // GESTURECONTROLLER_H
