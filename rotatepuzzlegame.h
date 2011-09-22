#ifndef ROTATEPUZZLEGAME_H
#define ROTATEPUZZLEGAME_H

#include "abstractpixmapwidget.h"
#include "connections.h"


class QPainter;
class QTimer;
class Ball;
class CoreController;
class EffectPainter;
class GestureController;
class SixtyOneGameBoardInfo;
class AbstractRule;
class RotatePuzzleGameRule;
class AbstractItem;
class IntegerItem;
class ExitItem;

/**
 * @brief A class to play a rotate puzzle game.
 */
class RotatePuzzleGame : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   *
   * @param ballIndex The original balls' index.
   * @param tobeIndex The shape of balls' index should be.
   * @param gameIndex Level of the game.
   * @param gameType Type of the game.
   * @param minSteps Minimal steps to complete the puzzle.
   */
  RotatePuzzleGame(int ballIndex[], int tobeIndex[],
                   int gameIndex, int gameType, int minSteps);

  /**
   * @brief Destructor.
   */
  ~RotatePuzzleGame();

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
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus();

private:
  // similar to other game mode
  AbstractRule *rule;
  SixtyOneGameBoardInfo *gameboardInfo;
  CoreController *controller;
  GestureController *gestureController;
  EffectPainter *effectPainter;
  QTimer *t;
  int frameCount;
  int *completeIndex;
  int type;
  int index;

  // hint information
  IntegerItem *currentSteps;
  IntegerItem *minimalSteps;
  // exit button
  ExitItem *exitItem;

  // collect items
  QVector <AbstractItem *> myItems;

  int direction;
  int targetSize;

  void quitGame();

private slots:
  void advance();
  void successMoved();
};

#endif // ROTATEPUZZLEGAME_H
