#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include "abstractpixmapwidget.h"
#include <QVector>
#include <QPointF>
#include <QPixmap>

class AbstractItem;

class HelpWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  HelpWidget();
  ~HelpWidget();

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
  double dy;
  QPointF lastPos;
  QPixmap p;
  AbstractItem *itemAtPressPos;
  AbstractItem *exitItem;

  QVector<AbstractItem *> myItems;
};

#endif // HELPWIDGET_H
