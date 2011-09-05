#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

#include "abstractpixmapwidget.h"
#include <QVector>

class AbstractItem;
class IntegerItem;
class StringItem;

class GameOverWidget : public AbstractPixmapWidget
{
public:
  GameOverWidget(int gameIndex, int score);
  ~GameOverWidget();

  // Functions most overloaded
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
//  virtual void init();
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus(){}

private:
  int index;

  AbstractItem *itemAtPressPos;
  IntegerItem *scoreHint;
  StringItem *newGameHint;
  AbstractItem *confirmItem;
  AbstractItem *cancelItem;

  QVector<AbstractItem *> myItems;
};

#endif // GAMEOVERWIDGET_H
