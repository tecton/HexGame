#ifndef ROTATECLASSICGAME_H
#define ROTATECLASSICGAME_H

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
class SwapClassicGameRule;
class SwapClassicGameSavedInfo;

class RotateClassicGame : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  RotateClassicGame();
  ~RotateClassicGame();

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
  int frameCount;
  int noSolutionCount;
  // TODO:

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
  void nextStage();

private slots:
  void advance();
  void dealStableEliminate(Connections connections);
  void dealUserMovingEliminate(Connections connections);
};

#endif // ROTATECLASSICGAME_H
