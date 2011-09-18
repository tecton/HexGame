#include "gesturecontroller.h"

#include <QPointF>
#include "abstractgameboardinfo.h"
#include "corecontroller.h"
#include "effectpainter.h"

GestureController::GestureController(AbstractRule *theRule,
                                     AbstractGameBoardInfo *theGameBoardInfo,
                                     CoreController *theController,
                                     EffectPainter *theEffectPainter) :
    rule(theRule),
    gameboardInfo(theGameBoardInfo),
    controller(theController),
    effectPainter(theEffectPainter),
    gesture(AbstractRule::BadGesture),
    gestureState(NoGesture)
{
}

void GestureController::testGesture(const QPointF& pos)
{
  if (gestureState != ChooseGesture)
    return;
  // Rotate
  if (rule->gestureAllowed(AbstractRule::Rotate) &&
      gestureIndexes.size() == 3)
  {
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

    // Clear
    gestureIndexes.clear();
    effectPainter->clearEffects();

    // If the center is valid
    if (gameboardInfo->canBeRotateCenter(center))
    {
      gestureState = LocateGesture;
      gesture = AbstractRule::Rotate;
      controller->rotateBegin(center, pos);
      return;
    }
  }
  // Swap
  else if (rule->gestureAllowed(AbstractRule::Swap) &&
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

    // Clear
    gestureIndexes.clear();
    effectPainter->clearEffects();

    // If the swap is valid
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
  // Clear
  gestureIndexes.clear();
  effectPainter->clearEffects();

  // Record the press
  gestureState = ChooseGesture;
  int index = gameboardInfo->indexOfPosition(pos);
  if (index >= 0)
  {
    gestureIndexes.push_back(index);
    effectPainter->selectAt(index);
  }
  else
    gestureState = NoGesture;
}

void GestureController::dealMoved(const QPointF& pos)
{
  // Record the move
  if (gestureState == ChooseGesture)
  {
    int index = gameboardInfo->indexOfPosition(pos);
    if (index >= 0)
    {
      if (gestureIndexes.size() == 0 ||
          (gestureIndexes.size() > 0 &&
           gestureIndexes[gestureIndexes.size() - 1] != index))
      {
        gestureIndexes.push_back(index);
        effectPainter->selectAt(index);
      }
    }

    // Test the gesture
    testGesture(pos);
  }
  if (gestureState == LocateGesture)
    controller->rotateTransition(pos);
}

void GestureController::dealReleased(const QPointF& pos)
{
  // End of the gesture
  if (gestureState == LocateGesture)
    if (gesture == AbstractRule::Rotate)
      controller->rotateEnd();
  gestureState = NoGesture;

  // Clear
  gestureIndexes.clear();
  effectPainter->clearEffects();
}
