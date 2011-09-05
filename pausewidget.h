#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

#include "abstractpixmapwidget.h"
#include <QVector>

class AbstractItem;
class StringItem;

class PauseWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  PauseWidget();
  ~PauseWidget();

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
  StringItem *pauseHint;

  QVector<AbstractItem *> myItems;

signals:
  void resume();
};

#endif // PAUSEWIDGET_H
