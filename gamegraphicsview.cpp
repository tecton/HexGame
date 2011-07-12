#include "gamegraphicsview.h"

GameGraphicsView::GameGraphicsView(QGraphicsScene *scene) :
    QGraphicsView(scene)
{

}

void GameGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
  //QGraphicsView::mouseMoveEvent(event);
  emit mouseMoved(event);
}

void GameGraphicsView::mousePressEvent(QMouseEvent *event)
{
  //QGraphicsView::mousePressEvent(event);
  emit mousePressed(event);
}

void GameGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
  //QGraphicsView::mouseReleaseEvent(event);
  emit mouseReleased(event);
}
