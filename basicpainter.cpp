#include "basicpainter.h"


#include <QVector>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>

#include "abstractitem.h"
#include "abstractgameboardinfo.h"
#include "ball.h"
#include "pixmapoperations.h"

const static int kBallsTotalColors = 8;
const static char * kBallsColorPaths[] = {":/images/balls/green*.png",
                                          ":/images/balls/red*.png",
                                          ":/images/balls/blue*.png",
                                          ":/images/balls/yellow*.png",
                                          ":/images/balls/purple*.png",
                                          ":/images/balls/white*.png",
                                          ":/images/balls/oringe*.png",
                                          ":/images/balls/brown*.png"};


QVector<QVector<QPixmap> > ballsPixmaps;
QVector<int> ballsFrameCounts;
void initBallsPixmaps()
{
  initPixmaps(kBallsTotalColors,
              kBallsColorPaths,
              ballsPixmaps,
              ballsFrameCounts);
}

const static int kBackgroundTotalColors = 1;
const static char * kBackgroundColorPaths[] = {":/images/backgrounds/mainmenubackground.png"};

QVector<QVector<QPixmap> > backgroundPixmaps;
QVector<int> backgroundFrameCounts;
void initBackgroundPixmaps()
{
  initPixmaps(kBackgroundTotalColors,
              kBackgroundColorPaths,
              backgroundPixmaps,
              backgroundFrameCounts);
}

void BasicPainter::paintBackGround(BackgroundId id,
                                   QPainter *painter,
                                   int width,
                                   int height,
                                   int frame)
{
  if (backgroundPixmaps.isEmpty())
    initBackgroundPixmaps();

  const QPixmap& p = backgroundPixmaps[id][frame % backgroundFrameCounts[id]];

  drawPixmapAt(painter,
               p,
               1.0 * width / p.width(),
               1.0 * height / p.height(),
               QPointF(0, 0),
               true,
               false);

}

void BasicPainter::paintBasicBalls(AbstractGameBoardInfo *gameboard,
                                   Ball **balls,
                                   int totalCount,
                                   QPainter *painter,
                                   double xRate,
                                   double yRate,
                                   int frame)
{
  double a = gameboard->ballR() * 2;

  if (ballsPixmaps.isEmpty())
    initBallsPixmaps();

  painter->setPen(QColor(255,255,255,255));

  for (int i = 0;i < totalCount;++i)
  {
    if (balls[i])
    {
      int colorIndex = balls[i]->getColor();
      const QPixmap& p = ballsPixmaps[colorIndex][frame % ballsFrameCounts[colorIndex]];
      QPointF pos = balls[i]->pos();
      pos.setX(pos.x() * xRate);
      pos.setY(pos.y() * yRate);
      drawPixmapAt(painter,
                   p,
                   a / p.width() * xRate,
                   a / p.height() * yRate,
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
  if (ballsPixmaps.isEmpty())
    initBallsPixmaps();

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
                   ballsPixmaps[colorIndex[i]][frame % ballsFrameCounts[colorIndex[i]]],
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
