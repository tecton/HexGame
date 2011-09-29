#include "corecontroller.h"

#include <QVector>
#include <QPointF>
#include <QDebug>
#include "abstractgameboardinfo.h"
#include "abstractrule.h"
#include "ball.h"
#include "connections.h"
#include "gamemath.h"

#define RECYCLE_STEPS 10

CoreController::CoreController(
    AbstractRule *theRule,
    AbstractGameBoardInfo *theGameBoardInfo,
    Ball **theBalls) :
  balls(theBalls),
  gameBoardInfo(theGameBoardInfo),
  rule(theRule),
  ballCount(theGameBoardInfo->totalBallCounts()),
  gestureCoolDown(0)
{
  // Create the space for the ball array if neccessary
  if (balls == NULL)
  {
    balls = new Ball *[ballCount];
    for (int i = 0;i < ballCount;++i)
      balls[i] = NULL;
  }

  // Create the space for the two arrays
  ballsCurrentIndexToOriginalIndex = new int[ballCount];
  ballsOriginalIndexToCurrentIndex = new int[ballCount];

  // Reset the 2 arrays above
  resetCToOAndOToC();

  // Create the space for the recycling balls
  recyclingBalls = new QVector<Ball *> *[RECYCLE_STEPS];
  for (int i = 0;i < RECYCLE_STEPS;++i)
    recyclingBalls[i] = new QVector<Ball *>();
}

void CoreController::resetCToOAndOToC()
{
  // Ball i is on the position i
  for (int i = 0;i < ballCount;++i)
  {
    ballsCurrentIndexToOriginalIndex[i] = i;
    ballsOriginalIndexToCurrentIndex[i] = i;
  }
}

Connections CoreController::testStableEliminate()
{
  // Get the result
  Connections result = testEliminate(false);

  // Emit the result
  emit stableEliminateTested(result);

  // Return the result
  return result;
}

Connections CoreController::testUserMovingEliminate()
{
  // Get the result
  Connections result = testEliminate(true);

  // Emit the result
  emit userMovingEliminateTested(result);

  // Return the result
  return result;
}

Connections CoreController::testEliminate(
    bool includingUserMoving)
{
  // Initialize the connect
  Connections result = Connections(ballCount);

  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
  {
    // If the ball doesn't exist, go to next ball
    if (!ballAt(i, includingUserMoving))
      continue;

    // Test 3 in a line
    Ball *firstBall = ballAt(i, includingUserMoving);
    for (int j = 0;j < 3;++j)
    {
      if (result.connectionsOfIndex[i][j] != NULL)
        continue;
      QVector<int> *currentConnection = new QVector<int>();
      int currentIndex = i;

      // A complex condition to judge whether the ball can
      // be in a connection and whether the ball has the
      // same color with the first ball
      while (currentIndex != -1 &&
             ballAt(currentIndex, includingUserMoving) &&
             firstBall->sameColor(
                 ballAt(currentIndex, includingUserMoving))
             &&
             (ballAt(currentIndex,
                     includingUserMoving)->getState() ==
              Ball::Stable ||
              ballAt(currentIndex,
                     includingUserMoving)->getState() ==
              Ball::AlmostStable ||
              ballAt(currentIndex,
                     includingUserMoving)->getState() ==
              Ball::UserReleased ||
              (includingUserMoving &&
               ballAt(currentIndex,
                      includingUserMoving)->getState() ==
               Ball::UserMoving)))
      {
        currentConnection->push_back(currentIndex);
        currentIndex =
            gameBoardInfo->nearbyIndex(currentIndex, j);
      }

      if (currentConnection->size() < 3)
        delete currentConnection;
      else
      {
        result.connections.push_back(currentConnection);
        for (int k = 0;k < currentConnection->size();++k)
          result.connectionsOfIndex
              [currentConnection->at(k)][j] =
              currentConnection;
      }
    }

    // Test 6 in a circle
    if (gameBoardInfo->canBeRotateCenter(i))
    {
      bool chainCanBeEliminated = true;
      QVector<int> chain =
          gameBoardInfo->chainAroundIndex(i);
      if (chain.size() == 6)
      {
        if (ballAt(chain[0], includingUserMoving))
        {
          Ball *firstBall = balls[chain[0]];
          for (int j = 0;j < 6;++j)
          {
            // A complex condition to judge whether the
            // ball can be in a connection and whether the
            // ball has the same color with the first ball
            if (!ballAt(chain[j], includingUserMoving) ||
                !firstBall->sameColor(ballAt(chain[j], includingUserMoving)) ||
                !(ballAt(chain[j], includingUserMoving)->getState() ==
                  Ball::Stable ||
                  ballAt(chain[j], includingUserMoving)->getState() ==
                  Ball::AlmostStable ||
                  ballAt(chain[j], includingUserMoving)->getState() ==
                  Ball::UserReleased ||
                  (includingUserMoving &&
                   ballAt(chain[j], includingUserMoving)->getState() ==
                   Ball::UserMoving)))
            {
              chainCanBeEliminated = false;
              break;
            }
          }
        }
        else
          chainCanBeEliminated = false;
      }
      else
        chainCanBeEliminated = false;
      if (chainCanBeEliminated)
      {
        // Record the chain
        QVector<int> *currentConnection =
            new QVector<int>(chain);
        result.connectionsOfIndex[i][3] =
            currentConnection;
        for (int j = 0;j < 6;++j)
          result.connectionsOfIndex[chain[j]][4+j/2] =
              currentConnection;
        result.connections.push_back(currentConnection);
      }
    }
  }

  return result;
}

QVector<QPointF> CoreController::newposUnderPos(
    QPointF mousePos)
{
  // The distance from the center
  double maxR = gameBoardInfo->intervalBetweenTwoLayers();
  QVector<QPointF> result =
      gestureInfluencedIndexsOriginalPos;

  // The position of the center
  QPointF centerPos = gameBoardInfo->positionOfIndex(rotateCenterIndex);

  // The position of the center of the gesture
  QPointF gestureCenterPos = centerPos;

  // Angles used to calculate the position
  double mouseOriginalA =
      angle(gestureConfirmPos, gestureCenterPos);
  double mouseCurrentA =
      angle(mousePos, gestureCenterPos);
  double dA = mouseCurrentA - mouseOriginalA;

  for (int i = 0;
       i < gestureInfluencedIndexsOriginalPos.size();
       ++i)
  {
    double a = angle(result[i], centerPos) + dA;
    double r = distanceFromTheCenterWithTheAngle(a, maxR);
    result[i] = calculatePosition(a, r, centerPos);
  }

  // Maintain the 2 arrays
  maintainCToOAndOToC(result[0]);

  return result;
}

int CoreController::maintainCToOAndOToC(QPointF firstPos)
{
  // The offset
  int offset = 0;

  // The min distance from the position given
  qreal minDis = 100000;
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    qreal currentDis = distanceOfTwoPoints
        (firstPos,
         gameBoardInfo->positionOfIndex(
           gestureInfluencedIndexs[i]));
    if (currentDis < minDis)
    {
      // Reset offset and maintain minDis if neccessary
      minDis = currentDis;
      offset = i;
    }
  }


  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    // Maintain the two arrays
    int o = gestureInfluencedIndexs[i];
    int c = gestureInfluencedIndexs[(i + offset) %
        gestureInfluencedIndexs.size()];
    ballsOriginalIndexToCurrentIndex[o] = c;
    ballsCurrentIndexToOriginalIndex[c] = o;
  }

  // Return the offset
  return offset;
}

void CoreController::eliminate(const QVector<int>& indexs)
{
  // The count of the balls eliminated
  int count = 0;
  for (int i = 0;i < indexs.size();++i)
  {
    int index = indexs.at(i);
    // If the ball exists
    if (balls[index])
    {
      ++count;
      recyclingBalls[RECYCLE_STEPS - 1]->
          push_back(balls[index]);
      balls[index] = NULL;
    }
  }
  emit eliminated(count);
}

void CoreController::rotateBegin(int theCenterIndex,
                                 QPointF theMousePosition)
{
  // If the operation is allowed
  if (!rule->gestureAllowed(AbstractRule::Rotate) ||
      !gameBoardInfo->canBeRotateCenter(theCenterIndex) ||
      gestureCoolDown != 0)
  {
    gesture =  AbstractRule::BadGesture;
    return;
  }

  // Record some infomation of the rotation
  rotateCenterIndex = theCenterIndex;
  gestureConfirmPos = theMousePosition;
  gesture = AbstractRule::Rotate;
  gestureInfluencedIndexs =
      gameBoardInfo->chainAroundIndex(rotateCenterIndex);
  gestureInfluencedIndexsOriginalPos.clear();
  // For each ball influenced,
  // if the ball doesn't exist or it's locked,
  // the gesture will be a bad one
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    if ((!balls[gestureInfluencedIndexs[i]]) ||
        (balls[gestureInfluencedIndexs[i]]->getLocked()) ||
        balls[gestureInfluencedIndexs[i]]->getState() != Ball::Stable)
    {
      gesture =  AbstractRule::BadGesture;
      gestureInfluencedIndexs.clear();
      return;
    }

  // Set the state of the balls
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    balls[gestureInfluencedIndexs[i]]->
        setState(Ball::UserMoving);
    gestureInfluencedIndexsOriginalPos.push_back(
          gameBoardInfo->positionOfIndex(
              gestureInfluencedIndexs[i]));
  }
}

void CoreController::rotateTransition(
    QPointF theMousePosition)
{
  if (gesture == AbstractRule::BadGesture ||
      !rule->gestureAllowed(AbstractRule::Rotate) ||
      gestureCoolDown != 0)
    return;

  // Get the new positions
  QVector<QPointF> newPos =
      newposUnderPos(theMousePosition);

  // For each ball, set the position
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    balls[gestureInfluencedIndexs[i]]->setPos(newPos[i]);

  // Test whether there is any connection
  testUserMovingEliminate();
}

void CoreController::rotateEnd()
{
  if (gesture == AbstractRule::BadGesture)
    return;

  // Get the offset
  int offset = maintainCToOAndOToC(
      balls[gestureInfluencedIndexs[0]]->pos());

  // If the balls are on the original position, roll back
  if (offset == 0)
  {
    rotateRollBack();
    gestureInfluencedIndexs.clear();
    return;
  }

  // For each ball influenced
  // Set the state
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    balls[gestureInfluencedIndexs[i]]->
        setState(Ball::UserReleased);

  // Set the balls to the new one
  Ball **tmp = new Ball*[gameBoardInfo->totalBallCounts()];
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    tmp[i] = balls[ballsCurrentIndexToOriginalIndex[i]];
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    balls[i] = tmp[i];

  Connections connections = testStableEliminate();

  bool rotateSuccessful =
      !rule->gameStepAllowed(AbstractRule::Eliminate);

  if (!rotateSuccessful)
  {
    // Test whether there is any connection
    for (int i = 0;i < gestureInfluencedIndexs.size();++i)
      if (connections.isInAChain(
          gestureInfluencedIndexs[i]))
      {
        rotateSuccessful = true;
        break;
      }
  }

  // If the rotate is successful
  if (rotateSuccessful)
  {
    if (rule->gameStepAllowed(AbstractRule::Eliminate))
    {
      QVector <int> toEliminate;
      for (int i = 0;
           i < gameBoardInfo->totalBallCounts();
           ++i)
        if (balls[i])
          if (connections.isInAChain(i))
            toEliminate.push_back(i);
      // Eliminate the balls
      eliminate(toEliminate);
    }

    // Move the balls left
    for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    {
      int index = gestureInfluencedIndexs[i];
      if (balls[index])
        translateABallTo(balls[index],
                         gameBoardInfo->
                         positionOfIndex(index),
                         2,
                         false);
    }
    // Emit a good move signal
    emit goodMove();
  }
  else
  {
    // Reset the balls
    for (int i = 0;
         i < gameBoardInfo->totalBallCounts();
         ++i)
      tmp[i] = balls[ballsOriginalIndexToCurrentIndex[i]];
    for (int i = 0;
         i < gameBoardInfo->totalBallCounts();
         ++i)
      balls[i] = tmp[i];
    rotateRollBack();
    // Emit a bad move signal
    emit badMove();
  }

  // Reset the two arrays
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int index = gestureInfluencedIndexs[i];
    ballsCurrentIndexToOriginalIndex[index] = index;
    ballsOriginalIndexToCurrentIndex[index] = index;
  }

  // Release the space
  delete [] tmp;
  gestureInfluencedIndexs.clear();
}

void CoreController::swap(int from, int to)
{
  // If a swap can be tried
  if (gestureCoolDown != 0)
    return;
  if ((!balls[from]) || balls[from]->getLocked())
    return;
  if ((!balls[to]) || balls[to]->getLocked())
    return;

  // Record the 2 balls
  ballsCurrentIndexToOriginalIndex[from] = to;
  ballsCurrentIndexToOriginalIndex[to] = from;
  ballsOriginalIndexToCurrentIndex[from] = to;
  ballsOriginalIndexToCurrentIndex[to] = from;
  gestureInfluencedIndexs.clear();
  gestureInfluencedIndexs.push_back(from);
  gestureInfluencedIndexs.push_back(to);

  // Set the state of the 2 balls
  balls[from]->setState(Ball::UserReleased);
  balls[to]->setState(Ball::UserReleased);

  // Test whether the swap is successful
  bool swapSuccessful =
      !rule->gameStepAllowed(AbstractRule::Eliminate);
  if (!swapSuccessful)
  {
    Connections connections = testUserMovingEliminate();
    if (connections.isInAChain(from) ||
        connections.isInAChain(to))
      swapSuccessful = true;
  }

  if (swapSuccessful)
  {
    // Move 2 balls to the new position
    moveToNewPos();
    // Emit a good move signal
    emit goodMove();
  }
  else
  {
    // Roll back the 2 balls
    swapRollBack(from, to);
    // Emit a bad move signal
    emit badMove();
  }
}


void CoreController::flameAt(int index)
{
  // Get the index of the balls influenced
  QVector <int> influencedIndexs = gameBoardInfo->chainAroundIndex(index);
  influencedIndexs.push_back(index);
  // Eliminate the balls
  eliminate(influencedIndexs);
}

void CoreController::starAt(int index)
{
  // Get the index of the balls influenced
  QVector <int> influencedIndexs;
  influencedIndexs.push_back(index);
  for (int i = 0;i < 6;++i)
  {
    int current = gameBoardInfo->nearbyIndex(index, i);
    while (current != -1)
    {
      influencedIndexs.push_back(current);
      current = gameBoardInfo->nearbyIndex(current, i);
    }
  }
  // Eliminate the balls
  eliminate(influencedIndexs);
}

void CoreController::fillAllBlanks()
{
  if (!rule->gameStepAllowed(AbstractRule::AutoRotate))
    return;
  // Auto rotate at the very beginning
  autoRotate();
  if (!rule->gameStepAllowed(AbstractRule::FillWithNewBalls))
    return;

  // Get the indexes which are blank
  QList <int> blankIndexes;
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    if (!balls[i])
      blankIndexes.push_back(i);

  // Record the indexes locked at this time
  QList <int> lockedIndexes;

  int index;

  do
  {
    // Unlock the balls
    foreach (index, blankIndexes)
      if (balls[index])
        balls[index]->setLocked(false);
    lockedIndexes.clear();

    // Create the balls
    foreach (index, blankIndexes)
    {
      // Set the color of te ball and whether it's locked
      if (!balls[index])
        balls[index] = new Ball((Ball::Color)(rand() % 6));
      else
        balls[index]->setColor((Ball::Color)(rand() % 6));
      if (!rule->endlessFill())
      {
        bool setToLock = lockedIndexes.size() < 2 &&
                         ((rand() % 100) > 5);
        balls[index]->setLocked(setToLock);
        if (setToLock)
          lockedIndexes.push_back(index);
      }
    }
  } while (rule->endlessFill() && (!blankIndexes.isEmpty()) && hint() < 0);

  // Set the balls to the correct position
  foreach (index, blankIndexes)
    balls[index]->setPos(gameBoardInfo->positionOfIndex(
          gameBoardInfo->firstOfChain(index)));

  // Reset the 2 arrays
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
  {
    ballsOriginalIndexToCurrentIndex[i] = i;
    ballsCurrentIndexToOriginalIndex[i] = i;
  }
}

void CoreController::autoRotate()
{
  const QVector<QVector<int> >& cs =
      gameBoardInfo->chains();
  for (int i = 0;i < gameBoardInfo->chainNumber();++i)
  {
    const QVector<int>& originalChain = cs.at(i);
    int needRotateCount = 0;
    int currentFillPos = originalChain.size() - 1;
    for (int j = currentFillPos;j >= 0;--j)
    {
      Ball *ball = balls[originalChain.at(j)];
      // If the ball doesn't exist
      if (!ball)
      {
        ++needRotateCount;
        continue;
      }

      if (ball->getState() == Ball::Stable)
        ball->setPos(gameBoardInfo->positionOfIndex(originalChain.at(j)));

      switch (ball->getState())
      {
      case Ball::UserMoving:
      // Break the user's operation
        if (needRotateCount != 0)
        {
          rotateRollBack();
          gestureInfluencedIndexs.clear();
        }
      case Ball::UserReleased:
      // A stable ball
      case Ball::Stable:
      // An almost stable ball
      case Ball::AlmostStable:
      // A ball moved by the system
      case Ball::SystemMoving:
        if (needRotateCount == 0)
          break;
        {
          // Current ball index
          int current = originalChain.at(j);
          // Target position index
          int target = originalChain.at(
              j + needRotateCount);
          // Rotate the ball to the target position
          rotateABallTo(ball,
                        gameBoardInfo->
                        positionOfIndex(target),
                        gameBoardInfo->centerPos(),
                        5,
                        true,
                        i);
          // Set the balls
          balls[target] = balls[current];
          balls[current] = NULL;
        }
        break;
      case Ball::JustCreated:
        {
          // Start position index
          int current = originalChain.at(0);
          // Target position index
          int target = originalChain.at(
              j + needRotateCount);
          // Set the balls to the start position
          ball->setPos(gameBoardInfo->
                       positionOfIndex(current));
          // Rotate the ball to the target position
          rotateABallTo(ball,
                        gameBoardInfo->
                        positionOfIndex(target),
                        gameBoardInfo->centerPos(),
                        10,
                        true,
                        i);
        }
      }
    }
  }
}

void CoreController::advance()
{
  // Calculate the CD
  if (gestureCoolDown > 0)
    --gestureCoolDown;

  // Advance each ball
  for (int i = 0;i < ballCount;++i)
    if (balls[i])
      balls[i]->advance();

  if (rule->gameStepAllowed(AbstractRule::Eliminate) ==
      false)
    return;

  // Recalculate the recycling balls
  for (int i = 0;i < recyclingBalls[0]->size();++i)
    delete recyclingBalls[0]->at(i);
  delete recyclingBalls[0];
  for (int i = 0;i < RECYCLE_STEPS - 1;++i)
    recyclingBalls[i] = recyclingBalls[i + 1];
  recyclingBalls[RECYCLE_STEPS - 1] =
      new QVector<Ball *>();

  // Test the stable eliminations
  Connections connections = testStableEliminate();
  QVector <int> toEliminate;
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    if (balls[i])
      if (connections.isInAChain(i))
        toEliminate.push_back(i);

  // Eliminate the balls
  eliminate(toEliminate);

  // Fill all blanks
  fillAllBlanks();
}


void CoreController::moveToNewPos()
{
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int originalIndex = gestureInfluencedIndexs[i];
    if (balls[originalIndex])
    {
      // Translate the ball to the correct position
      translateABallTo(balls[originalIndex],
                       gameBoardInfo->positionOfIndex(
                         ballsOriginalIndexToCurrentIndex
                         [originalIndex]),
                       3,
                       true);
      // Set the state of the ball
      balls[gestureInfluencedIndexs[i]]->setState(Ball::SystemMoving);
    }
  }
  // Reset the balls
  Ball **tmpBalls = new Ball *[ballCount];
  for (int i = 0;i < ballCount;++i)
    tmpBalls[i] =
        balls[ballsCurrentIndexToOriginalIndex[i]];
  for (int i = 0;i < ballCount;++i)
    balls[i] = tmpBalls[i];
  delete [] tmpBalls;
  // Maintain the 2 arrays
  resetCToOAndOToC();
}

void CoreController::rotateRollBack()
{
  gesture = AbstractRule::BadGesture;
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int currentIndex = gestureInfluencedIndexs[i];
    // Rotate the ball to the correct position
    rotateABallTo(balls[gestureInfluencedIndexs[i]],
                  gameBoardInfo->
                  positionOfIndex(currentIndex),
                  gameBoardInfo->
                  positionOfIndex(rotateCenterIndex),
                  5,
                  false,
                  1);
    // Set the state of the ball
    balls[gestureInfluencedIndexs[i]]->
        setState(Ball::SystemMoving);
  }
  // Maintain the 2 arrays
  resetCToOAndOToC();
  // Set the CD
  gestureCoolDown = 5;
}

void CoreController::swapRollBack(int from, int to)
{
  // Some complex calculation to calculate the positions
  // the balls should be at
  int halfSteps = 5;
  QPointF fromPos = gameBoardInfo->positionOfIndex(from);
  double fromX = fromPos.x();
  double fromY = fromPos.y();
  QPointF toPos = gameBoardInfo->positionOfIndex(to);
  double toX = toPos.x();
  double toY = toPos.y();

  if (balls[from])
    balls[from]->stopPositions.clear();
  if (balls[to])
    balls[to]->stopPositions.clear();

  // Move to the new position
  for (int i = 0;i < halfSteps;++i)
  {
    if (balls[from])
      balls[from]->stopPositions.push_back(
            QPointF((fromX * (halfSteps - i) + toX * i) /
                    halfSteps,
                    (fromY * (halfSteps - i) + toY * i) /
                    halfSteps));
    if (balls[to])
      balls[to]->stopPositions.push_back(
            QPointF((toX * (halfSteps - i) + fromX * i) /
                    halfSteps,
                    (toY * (halfSteps - i) + fromY * i) /
                    halfSteps));
  }
  // Move back to the original position
  for (int i = 1;i < halfSteps;++i)
  {
    if (balls[from])
      balls[from]->stopPositions.push_back(
            balls[from]->stopPositions[halfSteps - i]);
    if (balls[to])
      balls[to]->stopPositions.push_back(
            balls[to]->stopPositions[halfSteps - i]);
  }
  // Maintain the 2 arrays
  resetCToOAndOToC();
  // Set the CD
  gestureCoolDown = halfSteps * 2;
}

void CoreController::translateABallTo(Ball *ball,
                                      QPointF des,
                                      int steps,
                                      bool plain)
{
  if (!ball)
    return;

  // Some simple calculation to calculate the positions
  // the ball should be at
  QPointF fromPos = ball->pos();
  double fromX = fromPos.x();
  double fromY = fromPos.y();
  double toX = des.x();
  double toY = des.y();

  for (int i = 0;i < steps;++i)
    ball->stopPositions.push_back(
        QPointF((fromX * i + toX * (steps - i)) / steps,
                (fromY * i + toY * (steps - i)) / steps));
}


void CoreController::rotateABallTo(Ball *ball,
                                   QPointF toPos,
                                   QPointF centerPos,
                                   int steps,
                                   bool forceFillDirection,
                                   int distance)
{
  if (!ball)
    return;

  //  QPointF lastPos = ball->pos();
  //  if (!ball->stopPositions.isEmpty())
  //    lastPos = ball->stopPositions[0];
  //  if (distanceOfTwoPoints(lastPos, toPos) < 3)
  //    return;

  // Some complex calculation to calculate the positions
  // the ball should be at

  // The distance from the center
  double maxR = gameBoardInfo->intervalBetweenTwoLayers();
  ball->stopPositions.clear();
  ball->setState(Ball::SystemMoving);
  QPointF fromPos = ball->pos();
  if (!ball->stopPositions.isEmpty())
    fromPos = ball->stopPositions[0];
  double currentA = angle(fromPos, centerPos);
  double finalA = angle(toPos, centerPos);

  double tmp = currentA + PI;
  if (tmp > 2 * PI)
    tmp -= 2 * PI;

  double clockwiseAngleDis = currentA - finalA;
  if (clockwiseAngleDis < 0)
    clockwiseAngleDis += 2 * PI;

  bool clockwise = forceFillDirection ||
                   (clockwiseAngleDis < PI);

  double& angleDis = clockwiseAngleDis;
  angleDis = clockwise ?
             clockwiseAngleDis :
             2 * PI - clockwiseAngleDis;

  ball->stopPositions.push_back(toPos);
  int j = steps - 1;
  for (int i = 1;i < steps;++i,--j)
  {
    // Calculate the angle and the distance and set the
    // position
    double tmpA = currentA + (angleDis * j / (steps - 1)) *
                  (clockwise ? -1 : 1);
    double tmpR =
        distance *
        distanceFromTheCenterWithTheAngle(tmpA, maxR);
    ball->stopPositions.push_back(calculatePosition(
        tmpA, tmpR, centerPos));
  }
}

int CoreController::hint()
{
  // Code made by VincentQiuuu!!!
  int totalBallCounts = gameBoardInfo->totalBallCounts();

  // A copy of the balls
  Ball **copiedBalls = new Ball*[totalBallCounts];
  for (int i = 0;i < totalBallCounts;++i)
    copiedBalls[i] = balls[i];

  // Whether the balls have benn tried
  bool triedIndexes[totalBallCounts];
  for (int i = 0;i < totalBallCounts;++i)
    triedIndexes[i] = false;

  // Try the balls one by one according to a random order
  for (int i = 0;i < totalBallCounts;++i)
  {
    int randIndex = rand() % (totalBallCounts - i);
    int p = -1;
    for (int j = 0;j < randIndex + 1;++j)
    {
      ++p;
      while (triedIndexes[p])
        ++p;
    }
    int tryingIndex = p;
    triedIndexes[tryingIndex] = true;

    // Test rotate
    if (rule->gestureAllowed(AbstractRule::Rotate) &&
        gameBoardInfo->canBeRotateCenter(tryingIndex))
    {
      // The chain around the ball
      int chain[6];
      for (int j = 0;j < 6;++j)
        chain[j] = gameBoardInfo->
                   nearbyIndex(tryingIndex, j);
      // Calculate whether all the balls exist and not
      // locked
      bool allOccupied = true;
      for (int j = 0;j < 6;++j)
      {
        if (chain[j] == -1 ||
            copiedBalls[chain[j]] == NULL ||
            copiedBalls[chain[j]]->getLocked())
        {
          allOccupied = false;
          break;
        }
      }
      if (allOccupied)
      {
        // For each offset
        for (int j = 0;j < 6;++j)
        {
          Ball *ball1 = copiedBalls[chain[0]];
          // Set the balls and check
          for (int k = 0;k < 5;++k)
            copiedBalls[chain[k]] =
                copiedBalls[chain[k+1]];
          copiedBalls[chain[5]] = ball1;
          if ( j != 6 )
            for (int k = 0;k < 6;++k)
              if (check(copiedBalls, chain[k]))
              {
                delete [] copiedBalls;
                return tryingIndex;
              }
        }
      }
    }

    // Test swap
    if ( rule->gestureAllowed(AbstractRule::Swap) &&
         copiedBalls[tryingIndex] != NULL &&
         (!copiedBalls[tryingIndex]->getLocked()))
    {
      // The balls in the 3 directions
      int swapping[3];
      swapping[0] = gameBoardInfo->
                    leftDownIndex(tryingIndex);
      swapping[1] = gameBoardInfo->
                    rightDownIndex(tryingIndex);
      swapping[2] = gameBoardInfo->rightIndex(tryingIndex);
      // For each direction
      for (int k = 0;k < 3;++k)
      {
        if ( swapping[k] != -1 &&
             copiedBalls[swapping[k]] != NULL &&
             (!copiedBalls[swapping[k]]->getLocked()))
        {
          // Set the balls and check
          Ball* t = copiedBalls[tryingIndex];
          copiedBalls[tryingIndex] = copiedBalls[swapping[k]];
          copiedBalls[swapping[k]] = t;
          if (check(copiedBalls, tryingIndex))
          {
            delete [] copiedBalls;
            return swapping[k];
          }
          if (check(copiedBalls, swapping[k]))
          {
            delete [] copiedBalls;
            return tryingIndex;
          }
          t = copiedBalls[tryingIndex];
          copiedBalls[tryingIndex] = copiedBalls[swapping[k]];
          copiedBalls[swapping[k]] = t;
        }
      }
    }
  }

  delete [] copiedBalls;
  return -1;
}

bool CoreController::check(Ball** copiedBalls, int tryingIndex)
{
  for (int i = 0;i < 3;++i)
  {
    int lenOfChain = 1;
    for (int j = 0;j < 2;++j)
    {
      int movingIndex = tryingIndex;
      while (true)
      {
        int nextIndex =
            gameBoardInfo->
            nearbyIndex(movingIndex, i + 3 * j);
        if (nextIndex == -1 ||
            copiedBalls[nextIndex] == NULL ||
            (!copiedBalls[tryingIndex]->
             sameColor(copiedBalls[nextIndex])))
          break;
        ++lenOfChain;
        movingIndex = nextIndex;
      }
    }
    if (lenOfChain >= 3)
      return true;
  }

  int centers[6];
  for (int i = 0;i < 6;++i)
  {
    centers[i] = gameBoardInfo->nearbyIndex(tryingIndex, i);
    if ( gameBoardInfo->canBeRotateCenter(centers[i]) )
    {
      bool result = true;
      int firstNearbyIndex = gameBoardInfo->nearbyIndex(centers[i],0);
      if ( copiedBalls[firstNearbyIndex] != NULL )
      {
        for (int j = 1;j < 6;++j)
        {
          int nearbyIndex = gameBoardInfo->nearbyIndex((centers[i]), j);
          if ( copiedBalls[nearbyIndex] == NULL ||
               (!copiedBalls[nearbyIndex]->sameColor(copiedBalls[firstNearbyIndex])))
          {
            result = false;
            break;
          }
        }
        if ( result )
          return true;
      }
    }
  }
  return false;
}
