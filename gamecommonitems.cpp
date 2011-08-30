#include "gamecommonitems.h"

#include <QPainter>
#include "abstractgameboardinfo.h"
#include "effectpainter.h"

VerticalProgressBarItem::VerticalProgressBarItem()
{
  setMin(0);
  setMax(30);
  setCurrent(0);
}

void VerticalProgressBarItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  int percentage = (getCurrent() - getMin()) * 100 / (getMax() - getMin());
  painter->drawText(QPointF(x, y),
                    QObject::tr("%1 % (%2-%3)").
                    arg(percentage).
                    arg(getCurrent()).
                    arg(getMax()));
}

FlameItem::FlameItem()
{
  setMax(99);
  setCurrent(0);
}


void FlameItem::paint(QPainter *painter,
                      int width,
                      int height,
                      int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawText(QPointF(x, y),
                    QObject::tr("Flame * %1").
                    arg(getCurrent()));
}

void FlameItem::paintLocatingIcon(QPainter *painter,
                                  QPointF pos,
                                  int frame)
{
  painter->setPen(QColor(255,255,255,128));
  painter->drawText(pos, "(Flame)");
}

void FlameItem::paintInfluencedArea(int index,
                                    AbstractGameBoardInfo *gameBoard,
                                    EffectPainter *effectPainter,
                                    int frame)
{
  if (index == -1)
    return;
  effectPainter->bonusEliminationHintAt(index);
  QVector<int> chainAroundIndex = gameBoard->chainAroundIndex(index);
  for (int i = 0;i < chainAroundIndex.size();++i)
    effectPainter->bonusEliminationHintAt(chainAroundIndex[i]);
}

StarItem::StarItem()
{
  setMax(99);
  setCurrent(0);
}


void StarItem::paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawText(QPointF(x, y),
                    QObject::tr("Star  * %1").
                    arg(getCurrent()));
}

void StarItem::paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame)
{
  painter->setPen(QColor(255,255,255,128));
  painter->drawText(pos, "(Star)");
}

void StarItem::paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame)
{
  if (index == -1)
    return;
  effectPainter->bonusEliminationHintAt(index);
  for (int i = 0;i < 6;++i)
  {
    int current = gameBoard->nearbyIndex(index, i);
    while (current != -1)
    {
      effectPainter->bonusEliminationHintAt(current);
      current = gameBoard->nearbyIndex(current, i);
    }
  }
}

void HintItem::paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawText(QPointF(x, y), "Hint");
}

void ExitToMainMenuItem::paint(QPainter *painter,
                               int width,
                               int height,
                               int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawText(QPointF(x, y), "Exit");
}

void IntegerItem::paint(QPainter *painter,
                        int width,
                        int height,
                        int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawText(QPointF(x, y), QObject::tr("%1").arg(getValue()));
}
