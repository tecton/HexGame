#ifndef RESETWIDGET_H
#define RESETWIDGET_H

#include "abstractpixmapwidget.h"
#include <QVector>

class AbstractItem;

class ResetWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  ResetWidget();
  ~ResetWidget();

  // Functions most overloaded
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
//  virtual void init();
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);

private:
  AbstractItem *itemAtPressPos;
  AbstractItem *confirmItem;
  AbstractItem *cancelItem;
  QVector<AbstractItem *> myItems;

signals:
  void confirm();
  void cancel();
};

#endif // RESETWIDGET_H
