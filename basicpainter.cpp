#include "basicpainter.h"


#include <QVector>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>

#include "abstractitem.h"
#include "ball.h"
#include "pixmapoperations.h"

const static int kTotalColors = 8;
const static char * kColorPaths[] = {":/images/balls/red*.png",
                                     ":/images/balls/blue*.png",
                                     ":/images/balls/green*.png",
                                     ":/images/balls/yellow*.png",
                                     ":/images/balls/purple*.png",
                                     ":/images/balls/white*.png",
                                     ":/images/balls/oringe*.png",
                                     ":/images/balls/brown*.png"};


QVector<QVector<QPixmap> > pixmaps;
QVector<int> frameCounts;
void initPixmaps()
{
  initPixmaps(kTotalColors,
              kColorPaths,
              pixmaps,
              frameCounts);
}

void BasicPainter::paintBasicBalls(Ball **balls,
                                   int totalCount,
                                   QPainter *painter,
                                   double xRate,
                                   double yRate,
                                   int frame)
{
  if (pixmaps.isEmpty())
    initPixmaps();

  painter->setPen(QColor(255,255,255,255));

  for (int i = 0;i < totalCount;++i)
  {
    if (balls[i])
    {
      QPointF pos = balls[i]->pos();
      pos.setX(pos.x() * xRate);
      pos.setY(pos.y() * yRate);
      int colorIndex = balls[i]->getColor();
      drawPixmapAt(painter,
                   pixmaps[colorIndex][frame % frameCounts[colorIndex]],
                   xRate,
                   yRate,
                   pos,
                   true,
                   false);

      if (balls[i]->getLocked())
        painter->drawText(pos, "L");
    }
  }
}

void BasicPainter::paintPuzzleGameBalls(Ball **balls,
                                        int *colorIndex,
                                        int totalCount,
                                        QPainter *painter,
                                        double xRate,
                                        double yRate,
                                        int frame)
{
  if (pixmaps.isEmpty())
    initPixmaps();

  painter->setPen(QColor(255,255,255,255));

  for (int i = 0;i < totalCount;++i)
  {
    if (balls[i])
    {
      if (colorIndex[i] == -1)
        colorIndex[i] = 6;
      QPointF pos = balls[i]->pos();
      pos.setX(pos.x() * xRate);
      pos.setY(pos.y() * yRate);
      drawPixmapAt(painter,
                   pixmaps[colorIndex[i]][frame % frameCounts[colorIndex[i]]],
                   xRate,
                   yRate,
                   pos,
                   true,
                   false);
//      painter->drawText(pos, QObject::tr("%1").arg(i));
    }
  }
}

void BasicPainter::paintItems(QPainter *painter,
                              QVector<AbstractItem *> items,
                              int width,
                              int height,
                              int frame)
{
  for (int i = 0;i < items.size();++i)
    items[i]->paint(painter, width, height, frame);
}
