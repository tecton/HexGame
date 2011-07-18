/*
** A class inherit from
**   QWidget(in order to be a QWidget-.-)
**
** GameViewWidget runs the game~~~
**
*/

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
  // The view
  GameGraphicsView *view;

  // The scene
  QGraphicsScene *scene;

  // The balls
  PixmapItem *balls[TOTAL_ITEM_NUMBER];

  // Two arrays used when the user moves the balls
  // They tell what's the current position of the balls
  // (Haven't maintained correctly yet!!!!)
  int ballsCurrentIndexToOriginalIndex[TOTAL_ITEM_NUMBER];
  int ballsOriginalIndexToCurrentIndex[TOTAL_ITEM_NUMBER];

  // The timer
  QTimer *t;

  // The gesture
  GESTURE _gesture;

  // The direction of the gesture
  GESTURE_DIRECTION _gesture_direction;

  // The state of the gesture
  GESTURE_STATE _gesture_state;

  // The postion when confirm the gesture, used to calculate the offset
  QPointF _gesture_confirm_pos;

  // The index of the center of the rotation
  int     _gesture_rotate_center_index;

  // The MOUSE position of the center of the rotation
  QPointF _gesture_rotate_center_pos;

  // The indexes of the gesture
  QVector <int> _gesture_indexes;

  // A valid gesture will move some balls, here are the balls influenced
  QVector <int> _gesture_influenced_indexes;

  // The original position of the balls influenced
  QVector <QPointF> _gesture_influenced_indexes_original_pos;


  bool _should_roll_back;
  bool _should_eliminate;

  // Whether the gesture is valid
  // Should maintain:
  //   _gesture_state,
  //   _gesture,
  //   _gesture_direction,
  //   _gesture_influenced_indexes,
  //   _gesture_influenced_indexes_original_pos
  //   _gesture_rotate_center_pos
  bool testGesture();


  bool testEliminate();

  void dealWithTestEliminateResult(QVector<QVector<int> *>& connections,
                                   QVector<int> *connectionsOfIndex[TOTAL_ITEM_NUMBER][7]);

  // The new positions the balls influenced should be under the given mouse
  // position
  QVector<QPointF> newposUnderPos(QPointF mousePos);

  // A function to fill all blanks
  // (currently, the checks doesn't have any effect)
  void fillAllBlanks(bool allowMoreMove = false,
                     bool withoutDirectElimination = false);

  // Rotate the gameboard automatically
  void autoRotate();

  // Move to the new position after a valid gesture
  void moveToNewPos();

  // Maintain ballsCurrentIndexToOriginalIndex and
  //          ballsOriginalIndexToCurrentIndex
  // after calculate new position of the balls influenced
  void maintainCToOAndOToC(QPointF firstPos);

signals:

public slots:
  // Advance
  void advance();

  // 3 functions to deal with the mouse events
  // Press
  //   maintain:
  //     _gesture_state,
  //     _gesture_indexes
  void dealPressed(QMouseEvent *event);
  // Move
  //   if haven't confirm which gesture it is, append _gesture_indexes
  //   if the gesture is confirmed, set the new positions of each balls
  //   influenced
  void dealMoved(QMouseEvent *event);
  // Release
  //   put balls influenced to correct position
  //   exam whether it can be put there
  //   if can then do something
  //   if not then rollback
  void dealReleased(QMouseEvent *event);
};

#endif // GAMEVIEWWIDGET_H
