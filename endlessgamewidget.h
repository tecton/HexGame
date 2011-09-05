#ifndef ENDLESSGAMEWIDGET_H
#define ENDLESSGAMEWIDGET_H

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

class EndlessGameWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  EndlessGameWidget(AbstractRule::Gesture gesture);
  ~EndlessGameWidget();

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
  int frameCount;
  // TODO:

  IntegerItem *hightestScore;
  IntegerItem *currentLevel;
  AbstractProgressBarItem *progressBar;
  AbstractBonusItem *flame;
  AbstractBonusItem *star;
  AbstractItem *hint;
  AbstractItem *resetItem;
  AbstractItem *exitItem;

  QVector <AbstractItem *> myItems;

  AbstractItem *itemAtPressPos;
  QPointF currentPos;

  void showHint();
  void quitGame();
  void nextStage();

  int getIndex();


private slots:
  void advance();
  void reset();
  void elimitated(int count);
  void dealStableEliminate(Connections connections);
  void dealUserMovingEliminate(Connections connections);
};

#endif // ENDLESSGAMEWIDGET_H
