/*
** A class inherit from
**   QGraphicsView(in order to be a QGraphicsView-.-)
**
** GameGraphicsView emits 3 kinds of signals of the mouse events to deal with
** a bug(maybe a bug, maybe it's that I haven't find the correct way) in Qt
**
*/

#ifndef GAMEGRAPHICSVIEW_H
#define GAMEGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

class GameGraphicsView : public QGraphicsView
{
  Q_OBJECT
public:
  GameGraphicsView(QGraphicsScene *scene = NULL);

protected:
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

signals:
  void mouseMoved(QMouseEvent *event);
  void mousePressed(QMouseEvent *event);
  void mouseReleased(QMouseEvent *event);
};

#endif // GAMEGRAPHICSVIEW_H
