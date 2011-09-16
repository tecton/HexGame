/*
** A class to show the pause widget.
*/

#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>

// Forward declaration
class AbstractItem;
class StringItem;

class PauseWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  // Constructor
  PauseWidget();
  ~PauseWidget();

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
  // Items of the game
  StringItem *pauseHint;

  // A vector stores the items,
  // used to paint and release the space
  QVector<AbstractItem *> myItems;

signals:
  // A signal to tell other widget to resume
  void resume();
};

#endif // PAUSEWIDGET_H
