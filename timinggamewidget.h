#ifndef TIMINGGAMEWIDGET_H
#define TIMINGGAMEWIDGET_H

#include "abstractpixmapwidget.h"
#include "abstractrule.h"
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
class AbstractItem;
class AbstractBonusItem;
class IntegerItem;
class SwapClassicGameRule;
class SwapClassicGameSavedInfo;

class TimingGameWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  TimingGameWidget(AbstractRule::Gesture gesture);
  ~TimingGameWidget();

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
  AbstractGameBoardInfo *gameboardInfo;
  CoreController *controller;
  GestureController *gestureController;
  EffectPainter *effectPainter;
  QTimer *t;
  QTimer *oneSecondTimer;
  int frameCount;
  // TODO:

  IntegerItem *hightestScore;
  IntegerItem *currentScore;
  AbstractProgressBarItem *timeBar;
  AbstractBonusItem *flame;
  AbstractBonusItem *star;
  AbstractItem *hint;
  AbstractItem *resetItem;
  AbstractItem *pauseItem;
  AbstractItem *exitItem;

  QVector <AbstractItem *> myItems;

  AbstractItem *itemAtPressPos;
  QPointF currentPos;

  void showHint();
  void quitGame();
  void gameOver();

  int getIndex();


private slots:
  void advance();
  void resume();
  void reset();
  void oneSecond();
  void elimitated(int count);
  void dealStableEliminate(Connections connections);
  void dealUserMovingEliminate(Connections connections);
};

#endif // TIMINGGAMEWIDGET_H
