#include "gesturecontroller.h"

#include <QPointF>
#include "abstractgameboardinfo.h"
#include "corecontroller.h"

GestureController::GestureController(AbstractRule *theRule,
                                     AbstractGameBoardInfo *theGameBoardInfo,
                                     CoreController *theController) :
    rule(theRule),
    gameboardInfo(theGameBoardInfo),
    controller(theController),
    gesture(AbstractRule::BadGesture),
    gestureState(NoGesture)
{
}

void GestureController::testGesture(const QPointF& pos)
{
  if (gestureState != ChooseGesture)
    return;
  if (rule->gestureAllowed(AbstractRule::Rotate) &&
      gestureIndexes.size() == 3)
  { // Rotate
    // Try to find the center
    int *connectedCount = new int[gameboardInfo->totalBallCounts()];
    for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
      connectedCount[i] = 0;
    QVector<int> roundIndexes = gameboardInfo->chainAroundIndex(
                                    gestureIndexes[0]);
    for (int i = 0;i < roundIndexes.size();++i)
      ++connectedCount[roundIndexes[i]];
    roundIndexes = gameboardInfo->chainAroundIndex(
                                    gestureIndexes[1]);
    for (int i = 0;i < roundIndexes.size();++i)
      ++connectedCount[roundIndexes[i]];
    roundIndexes = gameboardInfo->chainAroundIndex(
                                    gestureIndexes[2]);
    for (int i = 0;i < roundIndexes.size();++i)
      ++connectedCount[roundIndexes[i]];
    int center = -1;
    for (int i = 0;i < gameboardInfo->totalBallCounts();++i)
      if (connectedCount[i] == 3)
      {
        center = i;
        break;
      }
    delete [] connectedCount;

    // If the center is valid
    if (gameboardInfo->canBeRotateCenter(center))
    {
      gestureState = LocateGesture;
      gesture = AbstractRule::Rotate;
      controller->rotateBegin(center, pos);
      return;
    }
  }
  if (rule->gestureAllowed(AbstractRule::Swap) &&
      gestureIndexes.size() == 2)
  {
    int from = gestureIndexes[0];
    int to = gestureIndexes[1];
    QVector<int> aroundFrom = gameboardInfo->chainAroundIndex(from);
    bool isNextTo = false;
    for (int i = 0;i < aroundFrom.size();++i)
      if (to == aroundFrom[i])
      {
        isNextTo = true;
        break;
      }
    if (isNextTo)
    {
      controller->swap(from, to);
      gestureState = NoGesture;
      return;
    }
  }
}

void GestureController::dealPressed(const QPointF& pos)
{
  gestureState = ChooseGesture;
  gestureIndexes.clear();
  int index = gameboardInfo->indexOfMousePosition(pos);
  if (index >= 0)
    gestureIndexes.push_back(index);
  else
    gestureState = NoGesture;
}

void GestureController::dealMoved(const QPointF& pos)
{
  if (gestureState == ChooseGesture)
  {
    int index = gameboardInfo->indexOfMousePosition(pos);
    if (index >= 0)
    {
      if (gestureIndexes.size() == 0 ||
          (gestureIndexes.size() > 0 &&
           gestureIndexes[gestureIndexes.size() - 1] != index))
        gestureIndexes.push_back(index);
    }
    testGesture(pos);
  }
  if (gestureState == LocateGesture)
    controller->rotateTransition(pos);
}

void GestureController::dealReleased(const QPointF& pos)
{
  if (gestureState == LocateGesture)
    if (gesture == AbstractRule::Rotate)
      controller->rotateEnd();
  gestureState = NoGesture;
}
