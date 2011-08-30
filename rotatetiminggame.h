#ifndef ROTATETIMINGGAME_H
#define ROTATETIMINGGAME_H

#include "abstractpixmapwidget.h"
#include "connections.h"

#include <QPointF>

class QPainter;
class QTimer;
class Ball;
class CoreController;
class EffectPainter;
class GestureController;
class AbstractGameBoardInfo;
class AbstractProgressBarItem;
class AbstractRule;
class AbstractItem;
class AbstractBonusItem;
class IntegerItem;
class SwapClassicGameRule;
class SwapClassicGameSavedInfo;

class RotateTimingGame : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  RotateTimingGame();
  ~RotateTimingGame();

  // Functions most overloaded
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
//  virtual void init();
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);

//  SwapClassicGameSavedInfo readSaved();

private:
  AbstractRule *rule;
  AbstractGameBoardInfo *gameboardInfo;
  CoreController *controller;
  GestureController *gestureController;
  EffectPainter *effectPainter;
  QTimer *t;
  QTimer *oneSecondTimer;
  int frameCount;
  // TODO:

  IntegerItem *scoreItem;
  AbstractProgressBarItem *progressBar;
  AbstractBonusItem *flame;
  AbstractBonusItem *star;
  AbstractItem *hint;
  AbstractItem *exitToMainMenu;

  QVector <AbstractItem *> myItems;

  AbstractItem *itemAtPressPos;
  QPointF currentPos;

  void showHint();
  void gameOver();
  void quitGame();

private slots:
  void advance();
  void eliminated(int count);
  void oneSecond();
  void dealStableEliminate(Connections connections);
  void dealUserMovingEliminate(Connections connections);
};

#endif // ROTATETIMINGGAME_H
