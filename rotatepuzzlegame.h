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

class RotatePuzzleGame : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /* ballIndex is the original balls' index
   * tobeIndex is the shape of balls' index which you should rotate to be
   * gameIndex indicates which level of game
   * gameType indicates which type of game
   * minSteps is the minimal steps to complete the puzzle */
  RotatePuzzleGame(int ballIndex[], int tobeIndex[],
                   int gameIndex, int gameType, int minSteps);
  ~RotatePuzzleGame();

  // Functions most overloaded
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus();

  // zoom in and out to view the target shape
  void zoomIn();
  void zoomOut();

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
