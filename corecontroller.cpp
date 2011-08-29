#include "corecontroller.h"

#include <QVector>
#include <QPointF>
#include <QDebug>
#include "abstractgameboardinfo.h"
#include "abstractrule.h"
#include "ball.h"
#include "connections.h"
#include "gamemath.h"

//AbstractGameBoardInfo *gameBoardInfo;
//AbstractRule *rule;
//Ball *balls;
//QPointF *currentPositions;
////bool *animMustFinish;

//QVector<Ball *> *recyclingBalls;

#define RECYCLE_STEPS 10

CoreController::CoreController(AbstractRule *theRule,
                               AbstractGameBoardInfo *theGameBoardInfo,
                               Ball **theBalls) :
  balls(theBalls),
  gameBoardInfo(theGameBoardInfo),
  rule(theRule),
  ballCount(theGameBoardInfo->totalBallCounts()),
  gestureCoolDown(0)
{
  if (balls == NULL)
  {
    balls = new Ball *[ballCount];
    for (int i = 0;i < ballCount;++i)
      balls[i] = NULL;
  }

  toBeShapeBalls = new Ball *[ballCount];
  for (int i = 0; i < ballCount; ++i)
  {
    if (balls[i] == NULL)
      toBeShapeBalls[i] = NULL;
    else
    {
      toBeShapeBalls[i] = new Ball();
      toBeShapeBalls[i]->setPos(gameBoardInfo->positionOfIndex(i));
    }
  }

  currentPositions = new QPointF[ballCount];
  ballsCurrentIndexToOriginalIndex = new int[ballCount];
  ballsOriginalIndexToCurrentIndex = new int[ballCount];
  resetCToOAndOToC();
  recyclingBalls = new QVector<Ball *> *[RECYCLE_STEPS];
  for (int i = 0;i < RECYCLE_STEPS;++i)
    recyclingBalls[i] = new QVector<Ball *>();
}

void CoreController::resetCToOAndOToC()
{
  for (int i = 0;i < ballCount;++i)
  {
    ballsCurrentIndexToOriginalIndex[i] = i;
    ballsOriginalIndexToCurrentIndex[i] = i;
  }
}

Connections CoreController::testStableEliminate()
{
  Connections result = testEliminate(false);
  emit stableEliminateTested(result);
  return result;
}

Connections CoreController::testUserMovingEliminate()
{
  Connections result = testEliminate(true);
  emit userMovingEliminateTested(result);
  return result;
}

Connections CoreController::testEliminate(bool includingUserMoving)
{
  Connections result = Connections(ballCount);

  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
  {
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
      while (currentIndex != -1 &&
             ballAt(currentIndex, includingUserMoving) &&
             firstBall->sameColor(ballAt(currentIndex, includingUserMoving)) &&
             (ballAt(currentIndex, includingUserMoving)->getState() == Ball::Stable ||
              ballAt(currentIndex, includingUserMoving)->getState() == Ball::AlmostStable ||
              ballAt(currentIndex, includingUserMoving)->getState() == Ball::UserReleased ||
              (includingUserMoving &&
               ballAt(currentIndex, includingUserMoving)->getState() == Ball::UserMoving)))
      {
        currentConnection->push_back(currentIndex);
        currentIndex = gameBoardInfo->nearbyIndex(currentIndex, j);
      }

      if (currentConnection->size() < 3)
        delete currentConnection;
      else
      {
        result.connections.push_back(currentConnection);
        for (int k = 0;k < currentConnection->size();++k)
          result.connectionsOfIndex[currentConnection->at(k)][j] =
              currentConnection;
      }
    }

    // Test 6 in a circle
    if (gameBoardInfo->canBeRotateCenter(i))
    {
      bool chainCanBeEliminated = true;
      QVector<int> chain = gameBoardInfo->chainAroundIndex(i);
      if (chain.size() == 6)
      {
        if (balls[chain[0]])
        {
          Ball *firstBall = balls[chain[0]];
          for (int j = 0;j < 6;++j)
          {
            if (!balls[chain[j]] ||
                !firstBall->sameColor(balls[chain[j]]))
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
        QVector<int> *currentConnection = new QVector<int>(chain);
        result.connectionsOfIndex[i][3] = currentConnection;
        for (int j = 0;j < 6;++j)
          result.connectionsOfIndex[chain[j]][4+j/2] = currentConnection;
        result.connections.push_back(currentConnection);
      }
    }
  }

  return result;
}

QVector<QPointF> CoreController::newposUnderPos(QPointF mousePos)
{
  double maxR = gameBoardInfo->intervalBetweenTwoLayers();
  QVector<QPointF> result = gestureInfluencedIndexsOriginalPos;

  QPointF centerPos = gameBoardInfo->positionOfIndex(rotateCenterIndex);
  QPointF gestureCenterPos = gameBoardInfo->centerPositionOfIndex(rotateCenterIndex);

  double mouseOriginalA = angle(gestureConfirmPos, gestureCenterPos);
  double mouseCurrentA = angle(mousePos, gestureCenterPos);
  double dA = mouseCurrentA - mouseOriginalA;

  for (int i = 0;i < gestureInfluencedIndexsOriginalPos.size();++i)
  {
    double a = angle(result[i], centerPos) + dA;
    double r = distanceFromTheCenterWithTheAngle(a, maxR);
    result[i] = calculatePosition(a, r, centerPos);
  }

  maintainCToOAndOToC(result[0]);

  return result;
}

void CoreController::maintainCToOAndOToC(QPointF firstPos)
{

  int offset = 0;
  qreal minDis = 100000;
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    qreal currentDis = distanceOfTwoPoints
        (firstPos,
         gameBoardInfo->positionOfIndex(
           gestureInfluencedIndexs[i]));
    if (currentDis < minDis)
    {
      minDis = currentDis;
      offset = i;
    }
  }


  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int o = gestureInfluencedIndexs[i];
    int c = gestureInfluencedIndexs[(i + offset) %
        gestureInfluencedIndexs.size()];
    ballsOriginalIndexToCurrentIndex[o] = c;
    ballsCurrentIndexToOriginalIndex[c] = o;
  }
}

void CoreController::eliminate(const QVector<int>& indexs)
{
  for (int i = 0;i < indexs.size();++i)
  {
    int index = indexs.at(i);
    if (balls[index])
    {
      recyclingBalls[RECYCLE_STEPS - 1]->push_back(balls[index]);
      balls[index] = NULL;
    }
  }
}

void CoreController::rotateBegin(int theCenterIndex,
                                 QPointF theMousePosition)
{
  if (!rule->gestureAllowed(AbstractRule::Rotate) ||
      !gameBoardInfo->canBeRotateCenter(theCenterIndex) ||
      gestureCoolDown != 0)
  {
    gesture =  AbstractRule::BadGesture;
    return;
  }
  rotateCenterIndex = theCenterIndex;
  gestureConfirmPos = theMousePosition;
  gesture = AbstractRule::Rotate;
  gestureInfluencedIndexs = gameBoardInfo->chainAroundIndex(rotateCenterIndex);
  gestureInfluencedIndexsOriginalPos.clear();
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    if ((!balls[gestureInfluencedIndexs[i]]) ||
        (balls[gestureInfluencedIndexs[i]]->getLocked()))
    {
      gesture =  AbstractRule::BadGesture;
      gestureInfluencedIndexs.clear();
      return;
    }
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    balls[gestureInfluencedIndexs[i]]->setState(Ball::UserMoving);
    gestureInfluencedIndexsOriginalPos.push_back(
          gameBoardInfo->positionOfIndex(gestureInfluencedIndexs[i]));
  }
}

void CoreController::rotateTransition(QPointF theMousePosition)
{
  if (gesture == AbstractRule::BadGesture ||
      !rule->gestureAllowed(AbstractRule::Rotate) ||
      gestureCoolDown != 0)
    return;

  QVector<QPointF> newPos = newposUnderPos(theMousePosition);
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    balls[gestureInfluencedIndexs[i]]->setPos(newPos[i]);

  testUserMovingEliminate();
}

void CoreController::rotateEnd()
{
  if (gesture == AbstractRule::BadGesture)
    return;
  maintainCToOAndOToC(balls[gestureInfluencedIndexs[0]]->pos());

  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    balls[gestureInfluencedIndexs[i]]->setState(Ball::UserReleased);

  Ball **tmp = new Ball *[gameBoardInfo->totalBallCounts()];
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    tmp[i] = balls[ballsCurrentIndexToOriginalIndex[i]];
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    balls[i] = tmp[i];

  Connections connections = testStableEliminate();
  bool rotateSuccessful = false || !rule->gameStepAllowed(AbstractRule::Eliminate);
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    if (connections.isInAChain(gestureInfluencedIndexs[i]))
    {
      rotateSuccessful = true;
      break;
    }
  if (rotateSuccessful)
  {
    if (rule->gameStepAllowed(AbstractRule::Eliminate))
    {
      QVector <int> toEliminate;
      for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
        if (balls[i])
          if (connections.isInAChain(i))
            toEliminate.push_back(i);
      eliminate(toEliminate);
    }
    for (int i = 0;i < gestureInfluencedIndexs.size();++i)
    {
      int index = gestureInfluencedIndexs[i];
      if (balls[index])
        translateABallTo(balls[index],
                         gameBoardInfo->positionOfIndex(index),
                         2,
                         false);
    }
    emit goodMove();
  }
  else
  {
    for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
      tmp[i] = balls[ballsOriginalIndexToCurrentIndex[i]];
    for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
      balls[i] = tmp[i];
    rotateRollBack();
    emit badMove();
  }
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int index = gestureInfluencedIndexs[i];
    ballsCurrentIndexToOriginalIndex[index] = index;
    ballsOriginalIndexToCurrentIndex[index] = index;
  }
  delete [] tmp;
  gestureInfluencedIndexs.clear();
}

void CoreController::swap(int from, int to)
{
  if (gestureCoolDown != 0)
    return;
  ballsCurrentIndexToOriginalIndex[from] = to;
  ballsCurrentIndexToOriginalIndex[to] = from;
  ballsOriginalIndexToCurrentIndex[from] = to;
  ballsOriginalIndexToCurrentIndex[to] = from;
  gestureInfluencedIndexs.clear();
  gestureInfluencedIndexs.push_back(from);
  gestureInfluencedIndexs.push_back(to);

  balls[from]->setState(Ball::UserReleased);
  balls[to]->setState(Ball::UserReleased);

  Connections connections = testUserMovingEliminate();
  bool swapSuccessful = false;
  if (connections.isInAChain(from) || connections.isInAChain(to))
    swapSuccessful = true;

  if (swapSuccessful)
  {
    moveToNewPos();
    emit goodMove();
  }
  else
  {
    swapRollBack(from, to);
    emit badMove();
  }
}


void CoreController::flameAt(int index)
{
  QVector <int> influencedIndexs = gameBoardInfo->chainAroundIndex(index);
  influencedIndexs.push_back(index);
  eliminate(influencedIndexs);
}

void CoreController::starAt(int index)
{
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
  eliminate(influencedIndexs);
}

void CoreController::fillAllBlanks()
{
  if (!rule->gameStepAllowed(AbstractRule::AutoRotate))
    return;
  autoRotate();
  if (!rule->gameStepAllowed(AbstractRule::FillWithNewBalls))
    return;
  QList <int> blankIndexes;
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    if (!balls[i])
      blankIndexes.push_back(i);
  do
  {
    for (QList <int>::Iterator itr = blankIndexes.begin();
         itr != blankIndexes.end();
         ++itr)
    {
      if (!balls[*itr])
        balls[*itr] = new Ball((Ball::Color)(rand() % 6));//0));//rand() % 8));
      else
        balls[*itr]->setColor((Ball::Color)(rand() % 6));//0))//rand() % 8));
      //      balls[*itr]->hide();
    }
  } while (false); // 这里面的要好好想想

  for (QList <int>::Iterator itr = blankIndexes.begin();
       itr != blankIndexes.end();
       ++itr)
    balls[*itr]->setPos(gameBoardInfo->positionOfIndex(*itr));
  //    balls[*itr]->show();


  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
  {
    ballsOriginalIndexToCurrentIndex[i] = i;
    ballsCurrentIndexToOriginalIndex[i] = i;
  }

  autoRotate();
}

void CoreController::autoRotate()
{
  const QVector<QVector<int> >& cs = gameBoardInfo->chains();
  for (int i = 0;i < gameBoardInfo->chainNumber();++i)
  {
    const QVector<int>& originalChain = cs.at(i);
    //QVector<int> rotateChain = doubleChain(originalChain);
    int needRotateCount = 0;
    int currentFillPos = originalChain.size() - 1;
    for (int j = currentFillPos;j >= 0;--j)
    {
      Ball *ball = balls[originalChain.at(j)];
      if (!ball) // 此格真没球
      {
        ++needRotateCount;
        continue;
      }
      //      if (ball->state() == PixmapItem::JUST_CREATED) // 此格还没填上来
      //        ++needRotateCount;
      switch (ball->getState())
      {
      case Ball::UserMoving:
      case Ball::UserReleased:
        // TODO 打断用户的操作
      case Ball::Stable: // 有稳定的球
      case Ball::AlmostStable:
      case Ball::SystemMoving: // 有正在转的球
        if (needRotateCount != 0)
        {
          int current = originalChain.at(j);
          int target = originalChain.at(j + needRotateCount);
          rotateABallTo(ball,
                        gameBoardInfo->positionOfIndex(target),
                        gameBoardInfo->centerPos(),
                        5,
                        true,
                        i);
          balls[target] = balls[current];
          balls[current] = NULL;
        }
        break;
      case Ball::JustCreated:
        int current = originalChain.at(0);
        int target = originalChain.at(j + needRotateCount);
        ball->setPos(gameBoardInfo->positionOfIndex(current));
        rotateABallTo(ball,
                      gameBoardInfo->positionOfIndex(target),
                      gameBoardInfo->centerPos(),
                      10,
                      true,
                      i);

      }
    }
  }
}

void CoreController::advance()
{
  if (gestureCoolDown > 0)
    --gestureCoolDown;
  for (int i = 0;i < ballCount;++i)
    if (balls[i])
      balls[i]->advance();
  if (rule->gameStepAllowed(AbstractRule::Eliminate) == false)
    return;
  //  for (int i = 0;i < recyclingBalls[0]->size();++i)
  //    delete recyclingBalls[0]->at(i);
  //  delete recyclingBalls[0];
  //  for (int i = 0;i < RECYCLE_STEPS - 1;++i)
  //    recyclingBalls[i] = recyclingBalls[i + 1];
  //  recyclingBalls[RECYCLE_STEPS - 1] = new QVector<Ball *>();

  Connections connections = testStableEliminate();
  QVector <int> toEliminate;
  for (int i = 0;i < gameBoardInfo->totalBallCounts();++i)
    if (balls[i])
      if (connections.isInAChain(i))
        toEliminate.push_back(i);
  eliminate(toEliminate);
  fillAllBlanks();
}


void CoreController::moveToNewPos()
{
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int originalIndex = gestureInfluencedIndexs[i];
    if (balls[originalIndex])
    {
      translateABallTo(balls[originalIndex],
                       gameBoardInfo->positionOfIndex(
                         ballsOriginalIndexToCurrentIndex[originalIndex]),
                       3,
                       true);
      balls[gestureInfluencedIndexs[i]]->setState(Ball::SystemMoving);
    }
  }
  Ball **tmpBalls = new Ball *[ballCount];
  for (int i = 0;i < ballCount;++i)
    tmpBalls[i] = balls[ballsCurrentIndexToOriginalIndex[i]];
  for (int i = 0;i < ballCount;++i)
    balls[i] = tmpBalls[i];
  delete [] tmpBalls;
  resetCToOAndOToC();
}

void CoreController::rotateRollBack()
{
  for (int i = 0;i < gestureInfluencedIndexs.size();++i)
  {
    int currentIndex = gestureInfluencedIndexs[i];
    rotateABallTo(balls[gestureInfluencedIndexs[i]],
                  gameBoardInfo->positionOfIndex(currentIndex),
                  gameBoardInfo->positionOfIndex(rotateCenterIndex),
                  5,
                  false,
                  1);
    balls[gestureInfluencedIndexs[i]]->setState(Ball::SystemMoving);
  }
  resetCToOAndOToC();
  gestureCoolDown = 5;
}

void CoreController::swapRollBack(int from, int to)
{
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

  for (int i = 0;i < halfSteps;++i)
  {
    if (balls[from])
      balls[from]->stopPositions.push_back(
            QPointF((fromX * (halfSteps - i) + toX * i) / halfSteps,
                    (fromY * (halfSteps - i) + toY * i) / halfSteps));
    if (balls[to])
      balls[to]->stopPositions.push_back(
            QPointF((toX * (halfSteps - i) + fromX * i) / halfSteps,
                    (toY * (halfSteps - i) + fromY * i) / halfSteps));
  }
  for (int i = 1;i < halfSteps;++i)
  {
    if (balls[from])
      balls[from]->stopPositions.push_back(
            balls[from]->stopPositions[halfSteps - i]);
    if (balls[to])
      balls[to]->stopPositions.push_back(
            balls[to]->stopPositions[halfSteps - i]);
  }
  resetCToOAndOToC();
  gestureCoolDown = halfSteps * 2;
}

void CoreController::translateABallTo(Ball *ball,
                                      QPointF des,
                                      int steps,
                                      bool plain)
{
  if (!ball)
    return;

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

  bool clockwise = forceFillDirection || (clockwiseAngleDis < PI);

  double& angleDis = clockwiseAngleDis;
  angleDis = clockwise ? clockwiseAngleDis : 2 * PI - clockwiseAngleDis;

  ball->stopPositions.push_back(toPos);
  int j = steps - 1;
  for (int i = 1;i < steps;++i,--j)
  {
    double tmpA = currentA + (angleDis * j / (steps - 1)) * (clockwise ? -1 : 1);
    double tmpR = distance * distanceFromTheCenterWithTheAngle(tmpA, maxR);
    ball->stopPositions.push_back(calculatePosition(tmpA, tmpR, centerPos));
  }
}

int CoreController::hint()
{
  int totalBallCounts = gameBoardInfo->totalBallCounts();
  Ball **copiedBalls = new Ball*[totalBallCounts];
  for (int i = 0;i < totalBallCounts;++i)
    copiedBalls[i] = balls[i];

  bool triedIndexes[totalBallCounts];
  for (int i = 0;i < totalBallCounts;++i)
    triedIndexes[i] = false;

  for (int i = 0;i < totalBallCounts;++i)
  {
    int randIndex = rand() % (totalBallCounts - i);
    int p = -1;
    for (int j = 0;j < randIndex + 1;++j)
    {
      ++p;
      while ( triedIndexes[p] )
        ++p;
    }
    int tryingIndex = p;
    triedIndexes[tryingIndex] = true;

    if ( rule->gestureAllowed(AbstractRule::Rotate) && gameBoardInfo->canBeRotateCenter(tryingIndex) )
    {
      int chain[6];
      for (int j = 0;j < 6;++j)
      {
        chain[j] = gameBoardInfo->nearbyIndex(tryingIndex, j);
      }
      bool allOccupied = true;
      for (int j = 0;j < 6;++j)
      {
        if ( copiedBalls[chain[j]] == NULL )
        {
          allOccupied = false;
          break;
        }
      }
      if ( allOccupied )
      {
        for (int j = 0;j < 6;++j)
        {
          Ball *ball1 = copiedBalls[chain[0]];
          for (int k = 0;k < 5;++k)
          {
            copiedBalls[chain[k]] = copiedBalls[chain[k+1]];
          }
          copiedBalls[chain[5]] = ball1;
          if ( j != 6 )
            for (int k = 0;k < 6;++k)
              if ( check(copiedBalls, chain[k]) )
                return tryingIndex;
        }
      }
    }

    if ( rule->gestureAllowed(AbstractRule::Swap) && copiedBalls[tryingIndex] != NULL )
    {
      int swapping[3];
      swapping[0] = gameBoardInfo->leftDownIndex(tryingIndex);
      swapping[1] = gameBoardInfo->rightDownIndex(tryingIndex);
      swapping[2] = gameBoardInfo->rightIndex(tryingIndex);
      for (int k = 0;k < 3;++k)
      {
        if ( swapping[k] != -1 && copiedBalls[swapping[k]] != NULL )
        {
          Ball* t = copiedBalls[tryingIndex];
          copiedBalls[tryingIndex] = copiedBalls[swapping[k]];
          copiedBalls[swapping[k]] = t;
          if ( check(copiedBalls, tryingIndex) )
            return tryingIndex;
          if ( check(copiedBalls, swapping[k]) )
            return swapping[k];
          t = copiedBalls[tryingIndex];
          copiedBalls[tryingIndex] = copiedBalls[swapping[k]];
          copiedBalls[swapping[k]] = t;
        }
      }
    }
  }
}

bool CoreController::check(Ball** copiedBalls, int tryingIndex)
{
  Ball::Color currentColor = copiedBalls[tryingIndex]->getColor();
  for (int i = 0;i < 3;++i)
  {
    int lenOfChain = 1;
    for (int j = 0;j < 2;++j)
    {
      int movingIndex = tryingIndex;
      while (1)
      {
        int nextIndex = gameBoardInfo->nearbyIndex(movingIndex, i + 3 * j);
        if ( nextIndex == -1 ||
          copiedBalls[nextIndex] == NULL ||
          copiedBalls[nextIndex]->getColor() != currentColor )
          break;
        ++lenOfChain;
        movingIndex = nextIndex;
      }
    }
    if ( lenOfChain >= 3 )
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
        Ball::Color chainColor = copiedBalls[firstNearbyIndex]->getColor();
        for (int j = 1;j < 6;++j)
        {
          int nearbyIndex = gameBoardInfo->nearbyIndex((centers[i]), j);
          if ( copiedBalls[nearbyIndex] == NULL ||
             copiedBalls[nearbyIndex]->getColor() != chainColor )
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
