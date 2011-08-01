/*
** A class to control the core part of the game.
*/

#ifndef CORECONTROLLER_H
#define CORECONTROLLER_H

#include "abstractrule.h"

#include <QVector>
#include <QPointF>
#include <QObject>
#include "connections.h"

class AbstractGameBoardInfo;
class Ball;
class Gesture;

class CoreController : public QObject
{
  Q_OBJECT
public:
  CoreController(AbstractRule *theRule,
                 AbstractGameBoardInfo *theGameBoardInfo,
                 Ball **theBalls = NULL);

//  void setBalls(Ball **theBalls);

  Connections testStableEliminate();
  Connections testUserMovingEliminate();

  void eliminate(const QVector<int>& indexs);

  void rotateBegin(int theCenterIndex, QPointF theMousePosition);
  void rotateTransition(QPointF theMousePosition);
  void rotateEnd();

  void swap(int from, int to);

  // A function to fill all blanks
  // (currently, the checks doesn't have any effect)
  void fillAllBlanks();

  // Rotate the gameboard automatically
  void autoRotate();

  void advance();

  Ball **balls;

private:
  AbstractGameBoardInfo *gameBoardInfo;
  AbstractRule *rule;
  int ballCount;
  QPointF *currentPositions;
  //bool *animMustFinish;

  QVector<Ball *> **recyclingBalls;

  int rotateCenterIndex;
  QPointF gestureConfirmPos;
  QVector<int> gestureInfluencedIndexs;
  QVector<QPointF> gestureInfluencedIndexsOriginalPos;

  // Two arrays used when the user moves the balls
  // They tell what's the current position of the balls
  // (Haven't maintained correctly yet!!!!)
  int *ballsCurrentIndexToOriginalIndex;
  int *ballsOriginalIndexToCurrentIndex;

  int gestureCoolDown;
  AbstractRule::Gesture gesture;

  void resetCToOAndOToC();
  Connections testEliminate(bool includingUserMoving = false);
  QVector<QPointF> newposUnderPos(QPointF mousePos);
  void moveToNewPos();
  void rotateRollBack();
  void swapRollBack(int from, int to);

  void translateABallTo(Ball *ball,
                        QPointF des,
                        int steps,
                        bool plain = false);
  void rotateABallTo(Ball *ball,
                     QPointF toPos,
                     QPointF centerPos,
                     int steps,
                     bool forceFillDirection,
                     int distance = 1);

  void maintainCToOAndOToC(QPointF firstPos);

  inline Ball *ballAt(int index, bool disorder)
  {
    return disorder ?
           balls[ballsCurrentIndexToOriginalIndex[index]] :
           balls[index];
  }

signals:
  void stableEliminateTested(Connections connections);
  void userMovingEliminateTested(Connections connections);
};

#endif // CORECONTROLLER_H
