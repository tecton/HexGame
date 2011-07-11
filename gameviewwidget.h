#ifndef GAMEVIEWWIDGET_H
#define GAMEVIEWWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>
#include <QPointF>
#include "pixmapitem.h"

class GameViewWidget : public QWidget
{
  Q_OBJECT
public:
  explicit GameViewWidget(QWidget *parent = 0);

protected:
  void mouseReleaseEvent(QMouseEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);

private:
  QGraphicsView *view;
  QGraphicsScene *scene;
  PixmapItem **balls;
  QTimer *t;
  int first;
  int direction;

  void fillAllBlanks(bool allowMoreMove = false,
                     bool withoutDirectElimination = false);
  void autoRotate();

signals:

public slots:
  void advance();
};

#endif // GAMEVIEWWIDGET_H
