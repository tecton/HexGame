#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include "gamelocation.h"

#include <QObject>
#include <QGraphicsItem>
#include <QPixmap>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QTimer>
#include <QVector>

class PixmapItem : public QObject, public QGraphicsItem
{
  Q_OBJECT
  Q_INTERFACES (QGraphicsItem)
public:

  enum BALL_COLOR{RED = 0, BLUE, GREEN, YELLOW, PURPLE, WHITE};
  enum STATE{STABLE, USER_MOVING, SYSTEM_MOVING, JUST_CREATED};

  PixmapItem(PixmapItem::BALL_COLOR color = PixmapItem::RED,
             QGraphicsScene *scene = 0);
  ~PixmapItem();


  QRectF boundingRect() const;
  QPainterPath shape() const;
  void paint(QPainter *painter,
             const QStyleOptionGraphicsItem *option,
             QWidget *widget);

  void setColor(PixmapItem::BALL_COLOR color);
  PixmapItem::BALL_COLOR color()
  {return _color;}

  void setState(PixmapItem::STATE state)
  {_state = state;}
  PixmapItem::STATE state()
  {return _state;}

  // Anims
  void rotateWithChain(const QVector<int>& chain,
                       int fromIndex,
                       int toIndex,
                       PixmapItem::STATE state,
                       int steps = 20,
                       bool append = true); //旋转到
  void translateTo(QPointF finalPos,
                   PixmapItem::STATE state,
                   int steps = 20,
                   bool plain = true,
                   bool append = true); //平移到
  void slideBackTo(QPointF fromPos,
                   QPointF toPos,
                   QPointF oneJointPos,
                   QPointF aontherJointPos,
                   PixmapItem::STATE state,
                   int steps = 20); // 从Slide的手势划回
  void disappearWithoutMove(int msec = 500);

  int leftAnimSteps()
  {return qMax(5, _stop_positions.size());}

  void advance(int phase);

private:
  // Basic things used when paint
  struct Frame {
      QPixmap pixmap;
      QPainterPath shape;
      QRectF boundingRect;
  };
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

signals:


//private slots:
//  void disappearOneStep();

public slots:
  void recycle();
  void del();
};


#endif // PIXMAPITEM_H
