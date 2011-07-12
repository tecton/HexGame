#ifndef GAMEVIEWWIDGET_H
#define GAMEVIEWWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMouseEvent>
#include <QPointF>
#include "pixmapitem.h"
#include "gamegraphicsview.h"

class GameViewWidget : public QWidget
{
  Q_OBJECT
public:
  explicit GameViewWidget(QWidget *parent = 0);

protected:
//  void mousePressEvent(QMouseEvent *);
//  void mouseReleaseEvent(QMouseEvent *);
//  void mouseDoubleClickEvent(QMouseEvent *);

private:
  GameGraphicsView *view;
  QGraphicsScene *scene;
  PixmapItem **balls;
  PixmapItem **ballsWithCurrentPos;
  int ballsCurrentIndexToOriginalIndex[TOTAL_ITEM_NUMBER];
  int ballsOriginalIndexToCurrentIndex[TOTAL_ITEM_NUMBER];
  QTimer *t;

  GESTURE _gesture;
  GESTURE_DIRECTION _gesture_direction;
  GESTURE_STATE _gesture_state;
  QPointF _gesture_confirm_pos;
  QPointF _gesture_rotate_center_pos;
  QVector <int> _gesture_indexes;
  QVector <int> _gesture_influenced_indexes;
  QVector <QPointF> _gesture_influenced_indexes_original_pos;
  bool testGesture(); // ���Դ����ƣ�
                      //   ��ȷ���ƣ�
                      //     ��_gesture_state��CHOOSE_GESTURE��ΪLOCATE_GESTURE
                      //     ��_gesture,
                      //       _gesture_direction,
                      //       _gesture_influenced_indexes,
                      //       _gesture_influenced_indexes_original_pos��Ϊ��ȷֵ
                      //     ��Ҫʱ��_gesture_rotate_center_pos��Ϊ��ȷֵ
  QVector<QPointF> newposUnderPos(QPointF mousePos);

  void fillAllBlanks(bool allowMoreMove = false,
                     bool withoutDirectElimination = false);
  void autoRotate();

signals:

public slots:
  void advance();
  void dealMoved(QMouseEvent *event);
  void dealPressed(QMouseEvent *event);
  void dealReleased(QMouseEvent *event);
};

#endif // GAMEVIEWWIDGET_H
