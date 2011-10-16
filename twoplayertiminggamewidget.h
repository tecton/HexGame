#ifndef TWOPLAYERTIMINGGAMEWIDGET_H
#define TWOPLAYERTIMINGGAMEWIDGET_H

#include "abstractpixmapwidget.h"
#include "abstractrule.h"
#include "connections.h"

// File must include
#include <QPointF>
#include <QList>
#include <QPixmap>
#include <QPainterPath>

// Forward declaration
class QPainter;
class QTimer;
class TimingGameWidget;

/**
 * @brief A class to play an endless game.
 */
class TwoPlayerTimingGameWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor with the gesture.
   */
  TwoPlayerTimingGameWidget(AbstractRule::Gesture gesture);

  /**
   * @brief Destructor.
   */
  ~TwoPlayerTimingGameWidget();

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
  virtual void loseForcus(){}

private:
  TimingGameWidget *game1, *game2;
  QPixmap game1End, game2End;
  QPainterPath youWin, youLose, drawGame;
  int result1, result2;
  int beginAnim;
  int endAnim;
  int frameCount;

  AbstractRule::Gesture ges;

  // A timer to send signals to advance the game
  QTimer *t;

  // A timer to send one signal every one second
  QTimer *oneSecondTimer;

  void start();

  QPointF gamePos(QPointF mousePos, int gameIndex);

private slots:
  // Advance
  void advance();

  // A game will emit this thing to show its end with a score
  void totalScore(TimingGameWidget *whoAmI, int score);
};

#endif // TWOPLAYERTIMINGGAMEWIDGET_H
