#ifndef CORECONTROLLER_H
#define CORECONTROLLER_H

// Files must include
#include <QVector>
#include <QPointF>
#include <QObject>
#include "abstractrule.h"
#include "connections.h"

// Forward declaration
class AbstractGameBoardInfo;
class Ball;
class Gesture;

/**
 * @brief A class to control the core part of the game.
 * It maintains all the things of the balls according to the rule and the
 * infomation of the gameboard.
 */
class CoreController : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief Constructor with the rule, the infomation of the gameboard and the
   * balls.
   */
  CoreController(AbstractRule *theRule,
                 AbstractGameBoardInfo *theGameBoardInfo,
                 Ball **theBalls = NULL);

  // Set the balls(no need to use it currently, so it
  // hasn't been realized)
//  void setBalls(Ball **theBalls);

  /**
   * @brief Eliminate the balls with the indexs.
   */
  void eliminate(const QVector<int>& indexs);

  /**
   * @brief Begin a rotate gesture.
   */
  void rotateBegin(int theCenterIndex,
                   QPointF theMousePosition);

  /**
   * @brief The rotate gesture continues and the balls move with the mouse.
   */
  void rotateTransition(QPointF theMousePosition);

  /**
   * @brief End the rotation.
   */
  void rotateEnd();

  /**
   * @brief Swap two balls.
   *
   * @param from The index of one of the ball.
   * @param to The index of one of the ball.
   */
  void swap(int from, int to);

  /**
   * @brief Use flame at the ball with the index.
   *
   * @param index The index of the ball.
   */
  void flameAt(int index);

  /**
   * @brief Use star at the ball with the index.
   *
   * @param index The index of the ball.
   */
  void starAt(int index);

  /**
   * @brief Fill all blanks.
   */
  void fillAllBlanks();

  /**
   * @brief Rotate the gameboard automatically.
   */
  void autoRotate();

  /**
   * @brief Advance the controller.
   *
   * @return The index of the key ball.(-1 when no solution)
   */
  void advance();

  /**
   * @brief The hint of the balls.
   */
  int hint();

  /**
   * @brief The balls.
   */
  Ball **balls;


  /**
   * @brief Translate a ball to a position.
   *
   * @param steps The steps the anim takes.
   * @param plain Whether the ball move directly to the position.
   */
  void translateABallTo(Ball *ball,
                        QPointF des,
                        int steps,
                        bool plain = true);

private:
  // Infomation of the gameboard
  AbstractGameBoardInfo *gameBoardInfo;

  // Rule
  AbstractRule *rule;

  // Total ball number
  int ballCount;

  // The balls which was recycled but may used later
  QVector<Ball *> **recyclingBalls;

  // Index of the center of the rotation
  int rotateCenterIndex;

  // The position when the rotate gesture is confirmed
  QPointF gestureConfirmPos;

  // The indexs of the balls the gesture influenced
  QVector<int> gestureInfluencedIndexs;

  // Original positions of the balls the gesture influenced
  // Used to speed up something
  QVector<QPointF> gestureInfluencedIndexsOriginalPos;

  // Two arrays used when the user moves the balls
  // They tell what's the current position of the balls
  // Maintained correctly by "maintainCToOAndOToC"
  int *ballsCurrentIndexToOriginalIndex;
  int *ballsOriginalIndexToCurrentIndex;

  // The cool down of the gesture.
  // After a wrong gesture is made, the controller will
  // ignore user's command for a while
  int gestureCoolDown;

  // The gesture used in the game
  AbstractRule::Gesture gesture;

  // Reset ballsCurrentIndexToOriginalIndex and
  //       ballsOriginalIndexToCurrentIndex
  void resetCToOAndOToC();

  /**
   * @brief Test elimination with balls which are stable.
   * @return The connections tested.
   */
  Connections testStableEliminate();

  /**
   * @brief Test elimination with balls which are stable and balls which are
   * moved by the user.
   * @return The connections tested.
   */
  Connections testUserMovingEliminate();

  // Realization of "testStableEliminate" and
  //                "testUserMovingEliminate"
  Connections testEliminate(bool includingUserMoving =
                            false);

  // Current position of the balls influenced by a gesture
  QVector<QPointF> newposUnderPos(QPointF mousePos);

  // Move the balls to the new position
  void moveToNewPos();

  // When a wrong rotate gesture is made, it is called to
  // roll back the balls
  void rotateRollBack();

  // When a wrong swap gesture is made, it is called to
  // roll back the balls
  void swapRollBack(int from, int to);

  // Rotate a ball to a position
  void rotateABallTo(Ball *ball,
                     QPointF toPos,
                     QPointF centerPos,
                     int steps,
                     bool forceFillDirection,
                     int distance = 1);

  // Maintain ballsCurrentIndexToOriginalIndex and
  //          ballsOriginalIndexToCurrentIndex
  int maintainCToOAndOToC(QPointF firstPos);

  // A function used to get a pointer of a ball
  inline Ball *ballAt(int index, bool disorder)
  {
    return disorder ?
           balls[ballsCurrentIndexToOriginalIndex[index]] :
           balls[index];
  }

  // Part of the "hint"
  bool check(Ball** copiedBalls, int tryingIndex);

signals:
  /**
   * @brief Emitted when "testStableEliminate" is called.
   * A game can get this signal and know which balls will
   * be eliminated and the structure of the connections.
   */
  void stableEliminateTested(Connections connections);

  /**
   * @brief Emitted when "testUserMovingEliminate" is called.
   * A game can get this signal and know which balls will
   * be eliminated if user ends the gesture and the
   * structure of the connections.
   */
  void userMovingEliminateTested(Connections connections);

  /**
   * @brief Emitted when "eliminate" is called.
   * A game can get this signal and know how many balls were eliminated.
   */
  void eliminated(int count);

  /**
   * @brief Emitted when a good move is made.
   */
  void goodMove();

  /**
   * @brief Emitted when a bad move is made.
   */
  void badMove();
};

#endif // CORECONTROLLER_H
