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
  RotatePuzzleGame(int ballIndex[], int tobeIndex[],
                   int gameIndex, int gameType, int minSteps);
  ~RotatePuzzleGame();

  // Functions most overloaded
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
//  virtual void init();
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus();

//  SwapClassicGameSavedInfo readSaved();

private:
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
  IntegerItem *currentSteps;
  IntegerItem *minimalSteps;
  ExitItem *exitItem;

  QVector <AbstractItem *> myItems;
  // TODO:分数、奖励之类的东西

  void quitGame();

private slots:
  void advance();
  void successMoved();
};

#endif // ROTATEPUZZLEGAME_H
