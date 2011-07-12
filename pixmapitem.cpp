#include <QPainter>
#include <QBitmap>
#include <QFileInfo>
#include <QDir>
#include <qmath.h>
#include "pixmapitem.h"

#define TOTAL_RECYCLE_STEPS 20

const static char * COLOR_PATHS[] = {":/images/balls/red*.png",
                                     ":/images/balls/blue*.png",
                                     ":/images/balls/green*.png",
                                     ":/images/balls/yellow*.png",
                                     ":/images/balls/purple*.png",
                                     ":/images/balls/white*.png"};

PixmapItem::PixmapItem(PixmapItem::BALL_COLOR color,
                       QGraphicsScene *scene):
  QObject(0),
  QGraphicsItem(0, scene),
  _current_frame(0),
  _color(color),
  _state(PixmapItem::JUST_CREATED),
  _bonus_point(0),
  _recycled(false)/*,
  _recycle_timer(new QTimer),
  _recycle_count(0)*//*,
  timer(new QTimeLine),
  anim(new QGraphicsItemAnimation)*/
{
//  timer->setFrameRange(0, 100);
//  anim->setItem(this);
//  anim->setTimeLine(timer);
  QString path(COLOR_PATHS[_color]);
  QFileInfo fi(path);
  foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
  {
    QPixmap currentPixmap = QPixmap(fi.path() + "/" + entry);
    Frame currentframe;
    currentframe.pixmap = currentPixmap;
    currentframe.shape = QPainterPath();
    currentframe.boundingRect = currentPixmap.rect();
    _frames << currentframe;
  }
}

void PixmapItem::setColor(PixmapItem::BALL_COLOR color)
{
  _frames.clear();
  _color = color;
  QString path(COLOR_PATHS[_color]);
  QFileInfo fi(path);
  foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
  {
    QPixmap currentPixmap = QPixmap(fi.path() + "/" + entry);
    Frame currentframe;
    currentframe.pixmap = currentPixmap;
    currentframe.shape = QPainterPath();
    currentframe.boundingRect = currentPixmap.rect();
    _frames << currentframe;
  }
}

QRectF PixmapItem::boundingRect() const
{
  return _frames.at(_current_frame).boundingRect;
}


QPainterPath PixmapItem::shape() const
{
  const Frame &f = _frames.at(_current_frame);
  if (f.shape.isEmpty()) {
    QPainterPath path;
    path.addRegion(f.pixmap.createHeuristicMask());
    const_cast<Frame &>(f).shape = path;
  }
  return f.shape;
}

void PixmapItem::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
  painter->drawPixmap(0,
                      0,
                      _frames.at(_current_frame).pixmap);
}

PixmapItem::~PixmapItem()
{
}

void PixmapItem::advance(int phase)
{
  if (phase == 1)
  {
    if (!_stop_positions.isEmpty())
    {
      setPos(_stop_positions.at(_stop_positions.size() - 1));
      _stop_positions.pop_back();
      if (_stop_positions.isEmpty()) // 若是为了流畅度，这里可以变成_stop_positions.size() < 某个数
        _state = STABLE;
    }
    if (_recycled)
    {
      setOpacity(_recycle_count * 1.0 / TOTAL_RECYCLE_STEPS);
      --_recycle_count;
      if (_recycle_count == 0)
        delete this;
    }
  }
}

void PixmapItem::rotateWithChain(const QVector<int>& chain,
                                 int fromIndex,
                                 int toIndex,
                                 PixmapItem::STATE state,
                                 int steps,
                                 bool append)
{
  QVector<QPointF> tmp;

  int fromIndexInChain = chain.indexOf(fromIndex);
  if (fromIndexInChain == -1)
    return;
  int toIndexInChain = chain.indexOf(toIndex, fromIndexInChain);
  if (toIndexInChain == -1)
    return;

  qreal indexPerStep = (toIndexInChain - fromIndexInChain + 1) * 1.0 / steps;

  for (int i = steps;i >= 0;--i)
  {
    int from = qMin(qFloor(indexPerStep * i + fromIndexInChain), toIndexInChain);
    int to = qMin(qCeil(indexPerStep * i + fromIndexInChain), toIndexInChain);
    if (from == to)
      tmp.push_back(QPointF(positionOfIndex(chain[from])));
    else
    {
      qreal toRate = (indexPerStep * i + fromIndexInChain - from) / (to - from);
      QPointF fromP = QPointF(positionOfIndex(chain[from]));
      QPointF toP = QPointF(positionOfIndex(chain[to]));
      qreal x = fromP.x() * (1 - toRate) + toP.x() * toRate;
      qreal y = fromP.y() * (1 - toRate) + toP.y() * toRate;
      tmp.push_back(QPointF(x,y));
    }
  }

  _state = state;

  if (append)
    for (int i = 0;i < _stop_positions.size();++i)
      tmp.push_back(_stop_positions[i]);
  _stop_positions = tmp;

//  QPointF currentPos = pos();
//  QPointF centerPos = positionOfIndex(centerIndex);
//  QPointF finalPos = positionOfIndex(finalIndex);

//  qreal currentR = distanceOfTwoPoints(currentPos, centerPos);
//  qreal finalR = distanceOfTwoPoints(finalPos, centerPos);

//  qreal currentA = angle(currentPos, centerPos);
//  qreal finalA = angle(finalPos, centerPos);

//  qreal tmp = currentA + PI;
//  if (tmp > 2 * PI)
//    tmp -= 2 * PI;

//  qreal clockwiseAngleDis = currentA - finalA;
//  if (clockwiseAngleDis < 0)
//    clockwiseAngleDis += 2 * PI;

//  clockwise = clockwise || clockwiseAngleDis < PI;

//  qreal& angleDis = clockwiseAngleDis;
//  angleDis = clockwise ? clockwiseAngleDis : 2 * PI - clockwiseAngleDis;

//  _stop_positions.clear();
//  int j = steps;
//  for (int i = 0; i <= steps; ++i,--j)
//  {
//    qreal tmpR = currentR + (finalR - currentR) * j / steps;
//    qreal tmpA = currentA + (angleDis * j / steps) * (clockwise ? -1 : 1);
//    _stop_positions.push_back(calculatePosition(tmpA, tmpR, centerPos));
//  }
}

void PixmapItem::translateTo(QPointF finalPos,
                             PixmapItem::STATE state,
                             int steps,
                             bool plain,
                             bool append)
{
  QVector<QPointF> tmp;
  QPointF currentPos = pos();

  qreal top = distanceOfTwoPoints(currentPos, finalPos);
  int j = steps;
  for (int i = 0; i <= steps; ++i,--j)
  {
    qreal tmpX = currentPos.x() + (finalPos.x() - currentPos.x()) * j / steps;
    qreal tmpY = currentPos.y() + (finalPos.y() - currentPos.y()) * j / steps;
    if (!plain)
      tmpY -= bridgeY(j * 1.0 / steps) * top;
    tmp.push_back(QPointF(tmpX, tmpY));
  }
  _state = state;

  if (append)
    for (int i = 0;i < _stop_positions.size();++i)
      tmp.push_back(_stop_positions[i]);
  _stop_positions = tmp;
}


void PixmapItem::slideBackTo(QPointF fromPos,
                             QPointF toPos,
                             QPointF oneJointPos,
                             QPointF anotherJointPos,
                             PixmapItem::STATE state,
                             int steps)
{
  _stop_positions.clear();
  qreal distanceDirect = distanceOfTwoPoints(fromPos, toPos);
  qreal distanceOneJoint = distanceOfTwoPoints(fromPos, oneJointPos) +
                           distanceOfTwoPoints(anotherJointPos, toPos);
  qreal distanceAnotherJoint = distanceOfTwoPoints(fromPos, anotherJointPos) +
                               distanceOfTwoPoints(oneJointPos, toPos);
  int choice = 0;
  if (distanceOneJoint <= distanceDirect && distanceOneJoint <= distanceAnotherJoint)
    choice = 1;
  else if (distanceAnotherJoint <= distanceDirect && distanceAnotherJoint <= distanceAnotherJoint)
    choice = 2;
  if (choice == 0)
    translateTo(toPos, state, steps);
  else
  {
    QPointF j1 = choice == 1 ? oneJointPos : anotherJointPos;
    QPointF j2 = choice == 2 ? oneJointPos : anotherJointPos;

    qreal firstStepDis = distanceOfTwoPoints(fromPos, j1);
    qreal secondStepDis = distanceOfTwoPoints(j2, toPos);

    int firstStepSteps = qMax(1.0, steps * firstStepDis / (firstStepDis + secondStepDis));
    int secondStepSteps = qMax(1, steps - firstStepSteps);

    QVector<QPointF> tmp;

    QPointF currentPos = j2;
    QPointF finalPos = toPos;
    int j = secondStepSteps;
    for (int i = 0; i <= secondStepSteps; ++i,--j)
    {
      qreal tmpX = currentPos.x() + (finalPos.x() - currentPos.x()) * j / secondStepSteps;
      qreal tmpY = currentPos.y() + (finalPos.y() - currentPos.y()) * j / secondStepSteps;
      tmp.push_back(QPointF(tmpX, tmpY));
    }

    currentPos = fromPos;
    finalPos = j1;
    j = firstStepSteps;
    for (int i = 0; i <= firstStepSteps; ++i,--j)
    {
      qreal tmpX = currentPos.x() + (finalPos.x() - currentPos.x()) * j / firstStepSteps;
      qreal tmpY = currentPos.y() + (finalPos.y() - currentPos.y()) * j / firstStepSteps;
      tmp.push_back(QPointF(tmpX, tmpY));
    }
    _stop_positions = tmp;
  }

}

void PixmapItem::recycle()
{
  _recycled = true;
  _recycle_count = TOTAL_RECYCLE_STEPS;
}

void PixmapItem::del()
{
  delete this;
}
