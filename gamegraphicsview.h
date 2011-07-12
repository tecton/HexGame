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
