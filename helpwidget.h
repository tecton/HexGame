/*
** Class of a help widget which shows the help.
*/

#ifndef HELPWIDGET_H
#define HELPWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>
#include <QPointF>
#include <QPixmap>

// Forward declaration
class AbstractItem;

class HelpWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  // Constructor
  HelpWidget();

  // Destructor
  ~HelpWidget();

  // Functions most overloaded
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus(){}

private:
  // The pixmap to show
  QPixmap p;

  // The deviation in y direction
  double dy;

  // The last position of the mouse
  QPointF lastPos;

  // Item of the game
  AbstractItem *exitItem;

  // A vector stores the items,
  // used to paint and release the space
  QVector<AbstractItem *> myItems;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos;
};

#endif // HELPWIDGET_H
