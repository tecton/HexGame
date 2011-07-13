/*
** A class inherit from
**   QObject(in order to use signals and slots mechanism)
**   QGraphicsItem(in order to be used in the Graphics View Framework)
**
** PixmapItem stores a few of pixmaps, show only one of them in a time,
** which may create an animation. (Pixmaps change and only change when
** it's "advance" function is called)
**
** PixmapItem also stores some infomation used in the game, such as the color,
** the state and the bonus. If neccessary, you can also add additional info to
** it.
**
** PixmapItem realized some animation may be used in the game, such as rotation
** and transportation.
**
*/

#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include "gamelocation.h"

#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QVector>
//#include <QGraphicsItemAnimation>
//#include <QTimeLine>
//#include <QTimer>

class PixmapItem : public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES (QGraphicsItem)
public:

  // Ball colors(must start from 0, because we will use the value as index)
  enum BALL_COLOR{RED = 0, BLUE, GREEN, YELLOW, PURPLE, WHITE};

  // Ball states
  //   STABLE:
  //     Ball is on a stable position.
  //   USER_MOVING:
  //     Ball is moved by the user.
  //   SYSTEM_MOVING:
  //     Ball is moved by the system.
  //   JUST_CREATED:
  //     Ball has just been created wating for additional operations.
  enum STATE{STABLE, USER_MOVING, SYSTEM_MOVING, JUST_CREATED};

  // Constructor, will call loadPixmaps()
  PixmapItem(PixmapItem::BALL_COLOR color = PixmapItem::RED,
             QGraphicsScene *scene = 0);

  // Destructor
  ~PixmapItem();

  // Functions must be overloaded
  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter,
             const QStyleOptionGraphicsItem *option,
             QWidget *widget);
  void advance(int phase);

  // Set the color, will call loadPixmaps()
  void setColor(PixmapItem::BALL_COLOR color);

  // Get the color
  PixmapItem::BALL_COLOR color()
  {return _color;}

  // Set the state
  void setState(PixmapItem::STATE state)
  {_state = state;}

  // Get the state
  PixmapItem::STATE state()
  {return _state;}

  // Load pixmaps
  void loadPixmaps();

  // Anims

  // Rotate with the chain given, positions should be in the cells which stores
  // the balls.
  // (In fact, you can also use this one to do some other moves, it's called
  //  "rotate" just because that the original purpose of creating the function
  //  is to do the rotation when filling the game board)
  //
  // Input:
  //   chain:
  //     A vector stores the indexes of the postion the ball should pass.
  //     (Because of the realization mechanism, it should be a doubled one
  //      like 1,2,3,1,2,3)
  //   fromIndex:
  //     Where should the rotation start from.
  //   toIndex:
  //     Where should the rotation end.
  //   state:
  //     What's the state of the ball during the rotation
  //     (supposed to be SYSTEM_MOVING, but who knows)
  //   steps:
  //     How many steps should the rotation last.
  //   append(may abandon this one later):
  //     Whether to append the current anim or break current one.
  void rotateWithChain(const QVector<int>& chain,
                       int fromIndex,
                       int toIndex,
                       PixmapItem::STATE state,
                       int steps = 20,
                       bool append = true); //Ðý×ªµ½

  // Translate to a position, you can choose the path is ^ or -.
  //
  // Input:
  //   finalPos:
  //     Where should the translation end.
  //   state:
  //     What's the state of the ball during the rotation
  //     (supposed to be SYSTEM_MOVING, but who knows)
  //   steps:
  //     How many steps should the rotation last.
  //   plain:
  //     If it's true, the path will be -, otherwise, the path is ^.
  //   append(may abandon this one later):
  //     Whether to append the current anim or break current one.
  void translateTo(QPointF finalPos,
                   PixmapItem::STATE state,
                   int steps = 20,
                   bool plain = true,
                   bool append = true);

  // Slide back to a position, choosing the nearest way, may choose to teleport
  // during the animation.
  // (In fact, you can also use this one to do some other moves, it's called
  //  "slideBackTo" just because that the original purpose of creating the
  //  function is to let the ball slide back to it's original position after
  //  a unsuccessfal SLIDE gesture)
  //
  // Input:
  //   4 "QPointF"s:
  //     The key positions, the ball will choose 1 from the 3 paths to move:
  //       fromPos-toPos
  //       fromPos-oneJointPos  aontherJointPos-toPos
  //       fromPos-aontherJointPos  oneJointPos-toPos
  //   state:
  //     What's the state of the ball during the rotation
  //     (supposed to be SYSTEM_MOVING, but who knows)
  //   steps:
  //     How many steps should the rotation last.
  void slideBackTo(QPointF fromPos,
                   QPointF toPos,
                   QPointF oneJointPos,
                   QPointF aontherJointPos,
                   PixmapItem::STATE state,
                   int steps = 20);

  // Rotate back to a position, choosing the nearest way.
  // (In fact, you can also use this one to do some other moves, it's called
  //  "rotateBackTo" just because that the original purpose of creating the
  //  function is to let the ball rotate back to it's original position after
  //  a unsuccessfal ROTATE gesture)
  //
  // Input:
  //   fromPos:
  //     Where should the rotation start from.
  //   toPos:
  //     Where should the rotation end.
  //   centerPos:
  //     The center of the rotation.
  //   state:
  //     What's the state of the ball during the rotation
  //     (supposed to be SYSTEM_MOVING, but who knows)
  //   steps:
  //     How many steps should the rotation last.
  // Warning:
  //   fromPos and toPos are only used to know the degree of rotation, not the
  //   distance, if you choose positions with improper distance, you will find
  //   the ball jump onto a track and finally jump to the toPos.
  void rotateBackTo(QPointF fromPos,
                    QPointF toPos,
                    QPointF centerPos,
                    PixmapItem::STATE state,
                    int steps = 20);

  // ABANDONED!!!
  // Abandoned because using many timers may make the program too slow under
  // MAC, and it may be a disaster to tablet.
  // Make the ball disappear without move.
  //
  // Input:
  //   msec:
  //     the time to disappear
//  void disappearWithoutMove(int msec = 500);

  // Steps the animation left.
  int leftAnimSteps()
  {return _stop_positions.size();}

private:
  // Basic infomation of a pixmap
  struct Frame {
      QPixmap pixmap;
      QPainterPath shape;
      QRectF boundingRect;
  };

  // The pixmaps
  QVector<Frame> _frames;
  int _current_frame;

  // Color, state, bonus point used in the game
  BALL_COLOR _color;
  STATE _state;
  int _bonus_point;

  // Things used to realize the anims
  QVector<QPointF> _stop_positions;
  bool _recycled;
  int _recycle_count;

//  QTimer *_recycle_timer;
//signals:

//private slots:
//  void disappearOneStep();

public slots:
  // Start to recycle this ball, it will delete the ball at the end
  // completely after TOTAL_RECYCLE_STEPS steps
  void recycle();

  // A slot to delete the ball, not used currently, but may be used later
  void del();
};

#endif // PIXMAPITEM_H
