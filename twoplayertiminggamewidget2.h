#ifndef TWOPLAYERTIMINGGAMEWIDGET2_H
#define TWOPLAYERTIMINGGAMEWIDGET2_H

#include "abstractpixmapwidget.h"
#include "abstractrule.h"
#include "connections.h"

// File must include
#include <QPointF>
#include <QList>
#include <QPainterPath>

// Forward declaration
class QPainter;
class QTimer;
class Ball;
class CoreController;
class EffectPainter;
class GestureController;
class AbstractGameBoardInfo;
class AbstractProgressBarItem;
class AbstractItem;
class AbstractBonusItem;
class IntegerItem;

class TwoPlayerTimingGameWidget2 : public AbstractPixmapWidget
{
public:

  Q_OBJECT
public:
  /**
   * @brief Constructor with the gesture.
   */
  TwoPlayerTimingGameWidget2(AbstractRule::Gesture gesture);

  /**
   * @brief Destructor.
   */
  ~TwoPlayerTimingGameWidget2();

  virtual void makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height);
  virtual void makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height);
  virtual void addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos,
                           Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos,
                         Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos,
                            Qt::MouseButton button);
  virtual void getForcus();
  virtual void loseForcus();
  virtual int suggestInterval()
  {return 60;}

private:
  // Rule of the game
  AbstractRule *rule;

  // Infomation of the gameboard
  AbstractGameBoardInfo *gameboardInfo;

  // Core controller which controls the balls
  CoreController *controller1;

  // Core controller which controls the balls
  CoreController *controller2;

  // Gesture controller which connects the gesture
  // of the user to the core controller
  GestureController *gestureController1;

  // Gesture controller which connects the gesture
  // of the user to the core controller
  GestureController *gestureController2;

  // A timer to send signals to advance the game
  QTimer *t;

  // A timer to send one signal every one second
  QTimer *oneSecondTimer;

  // Count of the frame which may used to paint
  int frameCount;

  // Items of the game
  IntegerItem *currentScore1;
  IntegerItem *currentScore2;
  AbstractProgressBarItem *timeBar;
  AbstractBonusItem *flame1;
  AbstractBonusItem *flame2;
  AbstractBonusItem *star1;
  AbstractBonusItem *star2;
  AbstractItem *resetItem;
  AbstractItem *pauseItem;
  AbstractItem *exitItem;

  // A vector stores the items,
  // used to paint and release the space
  QVector <AbstractItem *> myItems;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos1;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos2;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos;

  // Current position of the mouse,
  // used to show the hints
  QPointF currentPos1;

  // Current position of the mouse,
  // used to show the hints
  QPointF currentPos2;

  // Count of the start anim
  int startAnimCount;

  int endAnimCount;

  // Whether it's time up
  bool timeUp;

  // Count of the end anim for each bonus item
  QList<int> endAnimCount1;

  // Count of the end anim for each bonus item
  QList<int> endAnimCount2;

  // The kind of bonus item in end anim
  QList<int> endAnimBonusKind1;

  // The kind of bonus item in end anim
  QList<int> endAnimBonusKind2;

  QPainterPath youWin, youLose, drawGame;

  // Game over
  void gameOver();

  // Quit game
  void quitGame();

  QPointF game1ToGlobal(QPointF pos);
  QPointF game2ToGlobal(QPointF pos);

public slots:
  // Advance
  void advance();

private slots:
  // Reset
  void reset();

  // Deal stable eliminate
  // The connections are the balls which will be eliminated
  void dealStableEliminate1(Connections connections);

  // Deal stable eliminate
  // The connections are the balls which will be eliminated
  void dealStableEliminate2(Connections connections);

  // Called when some balls are eliminated
  void eliminated1(int count);

  // Called when some balls are eliminated
  void eliminated2(int count);

  // Called every one second
  void oneSecond();

  // Resume the game
  void resume();

  // Call after a good move is made
  void goodMove();

  // Call after a bad move is made
  void badMove();

};

#endif // TWOPLAYERTIMINGGAMEWIDGET2_H
