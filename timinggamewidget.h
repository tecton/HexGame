#ifndef TIMINGGAMEWIDGET_H
#define TIMINGGAMEWIDGET_H

#include "abstractpixmapwidget.h"
#include "abstractrule.h"
#include "connections.h"

// File must include
#include <QPointF>
#include <QList>

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

/**
 * @brief A class to play a timing game.
 */
class TimingGameWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor with the gesture.
   */
  TimingGameWidget(AbstractRule::Gesture gesture);

  /**
   * @brief Destructor.
   */
  ~TimingGameWidget();

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

  void setDoNotStop(bool b)
  {doNotStop = b;}

  void useGivenTimer(QTimer *aTimer, QTimer *oTimer);

  void setHaveBackground(bool b)
  {haveBackground = b;}

private:
  // Rule of the game
  AbstractRule *rule;

  // Infomation of the gameboard
  AbstractGameBoardInfo *gameboardInfo;

  // Core controller which controls the balls
  CoreController *controller;

  // Gesture controller which connects the gesture
  // of the user to the core controller
  GestureController *gestureController;

  // A painter to paint the effects of the game
  EffectPainter *effectPainter;

  // A timer to send signals to advance the game
  QTimer *t;

  // A timer to send one signal every one second
  QTimer *oneSecondTimer;

  // Count of the frame which may used to paint
  int frameCount;

  // Items of the game
  IntegerItem *hightestScore;
  IntegerItem *currentScore;
  AbstractProgressBarItem *timeBar;
  AbstractBonusItem *flame;
  AbstractBonusItem *star;
  AbstractItem *hint;
  AbstractItem *resetItem;
  AbstractItem *pauseItem;
  AbstractItem *exitItem;

  // A vector stores the items,
  // used to paint and release the space
  QVector <AbstractItem *> myItems;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos;

  // Current position of the mouse,
  // used to show the hints
  QPointF currentPos;

  // Count of the start anim
  int startAnimCount;

  // Whether it's time up
  bool timeUp;

  // Whether a user can't pause or reset
  bool doNotStop;

  // Whether the timers are my own
  bool myOwnTimers;

  // Whether to paint the background
  bool haveBackground;

  // Count of the end anim for each bonus item
  QList<int> endAnimCount;

  // The kind of bonus item in end anim
  QList<int> endAnimBonusKind;

  // Show the hint
  void showHint();

  // Game over
  void gameOver();

  // Quit game
  void quitGame();

  // Get the index of this game
  int getIndex();

public slots:
  // Advance
  void advance();

private slots:

  // Reset
  void reset();

  // Deal stable eliminate
  // The connections are the balls which will be eliminated
  void dealStableEliminate(Connections connections);

  // Deal user moving eliminate
  // The connections are the balls which will be eliminated
  // if user release the mouse
  void dealUserMovingEliminate(Connections connections);

  // Called when some balls are eliminated
  void eliminated(int count);

  // Called every one second
  void oneSecond();

  // Resume the game
  void resume();

  // Call after a good move is made
  void goodMove();

  // Call after a bad move is made
  void badMove();

signals:
  void totalScore(TimingGameWidget *whoAmI, int score);
};

#endif // TIMINGGAMEWIDGET_H
