/*
** Abstract class of a widget which can't be seen directly,
** but can make a pixmap to show what it should look like.
** Only one such widget is allowed to be active at a time,
** so it should emit a signal when it give this ability to
** other widget.
*/

#ifndef ABSTRACTPIXMAPWIDGET_H
#define ABSTRACTPIXMAPWIDGET_H

#include <QObject>

class QPixmap;
class QPointF;

class AbstractPixmapWidget :public QObject
{
  Q_OBJECT
public:
  // Make the pixmap with the given width and height
  virtual void makePixmap(QPixmap& pixmap, int width, int height)=0;

  // Make the basic part of the pixmap
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height)=0;

  // Add the effect to the pixmap
  virtual void addEffect(QPixmap& pixmap, int width, int height)=0;

  // Returns the logical position of the given rate
  virtual QPointF toScene(double xRate, double yRate)=0;

  // 3 functions to deal with mouse operations
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button)=0;
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button)=0;

signals:
  // The signal mentioned at beginning
  void giveControlTo(AbstractPixmapWidget *target, bool deleteMySelf);
};

#endif // ABSTRACTPIXMAPWIDGET_H
