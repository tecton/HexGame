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

// Total color number of the balls
const static int kBallsTotalColors = 8;

// File path of the balls
// (The last two are the same as two before them, may make
//  cause some bug later)
const static char * kBallsColorPaths[] =
{":/images/balls/red*.png",
 ":/images/balls/blue*.png",
 ":/images/balls/green*.png",
 ":/images/balls/yellow*.png",
 ":/images/balls/purple*.png",
 ":/images/balls/white*.png",
 ":/images/balls/purple*.png",
 ":/images/balls/white*.png"};

// Pixmaps of the balls
QVector<QVector<QPixmap> > ballsPixmaps;

// Total frame count of the balls
QVector<int> ballsFrameCounts;
void initBallsPixmaps()
{
  initPixmaps(kBallsTotalColors,
              kBallsColorPaths,
              ballsPixmaps,
              ballsFrameCounts);
}

// Total background number
const static int kBackgroundTotalColors = 5;
const static char * kBackgroundColorPaths[] =
{":/images/backgrounds/mainmenubackground.png",
 ":/images/backgrounds/puzzlemenubackground.png",
 ":/images/backgrounds/mainmenubackground.png",
 ":/images/backgrounds/37gamebackground.png",
 ":/images/backgrounds/61gamebackground.png"};

// Pixmaps of the backgrounds
QVector<QVector<QPixmap> > backgroundPixmaps;

// Total frame count of the backgrounds
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
  // Init backgrounds if neccessary
  if (backgroundPixmaps.isEmpty())
    initBackgroundPixmaps();

  // Get the pixmap
  const QPixmap& p = backgroundPixmaps[id]
                     [frame % backgroundFrameCounts[id]];

  // Draw the pixmap
  drawPixmapAt(painter,
               p,
               1.0 * width / p.width(),
               1.0 * height / p.height(),
               QPointF(0, 0),
               true,
               false);

}

// Pixmaps of the lock of the balls
QVector<QPixmap> ballsLockPixmaps;

void BasicPainter::paintBasicBalls(AbstractGameBoardInfo *
                                       gameboard,
                                   Ball **balls,
                                   int totalCount,
                                   QPainter *painter,
                                   double xRate,
                                   double yRate,
                                   int frame)
{
  // Size of the ball
  double size = gameboard->ballR() * 2;

  // Init balls if neccessary
  if (ballsPixmaps.isEmpty())
    initBallsPixmaps();

  // Init lock if neccessary
  if (ballsLockPixmaps.isEmpty())
  {
    // A value which won't be used
    int tmp;
    initPixmaps(":/images/balls/lock*.png",
                ballsLockPixmaps,
                tmp);

  }

  // Set the pen
  painter->setPen(QColor(255,255,255,255));

  // For each ball
  for (int i = 0;i < totalCount;++i)
  {
    // If the ball exists
    if (balls[i])
    {
      // Get the color
      int colorIndex = balls[i]->getColor();
      // Get the pixmap
      const QPixmap& p = ballsPixmaps[colorIndex]
          [frame % ballsFrameCounts[colorIndex]];
      // Get the position of the ball
      QPointF pos = balls[i]->pos();
      // Reset the position according to the rate
      pos.setX(pos.x() * xRate);
      pos.setY(pos.y() * yRate);
      // Draw the pixmap
      drawPixmapAt(painter,
                   p,
                   size / p.width() * xRate,
                   size / p.height() * yRate,
                   pos,
                   true,
                   true);

      // If the ball is locked
      if (balls[i]->getLocked())
      {
        // Get the pixmap
        const QPixmap& p2 = ballsLockPixmaps
            [frame % ballsLockPixmaps.size()];

        // Draw the pixmap
        drawPixmapAt(painter,
                     p2,
                     size / p2.width() * xRate * 0.75,
                     size / p2.height() * yRate * 0.75,
                     pos,
                     true,
                     true);
      }
    }
  }
}

void BasicPainter::paintPuzzleGameBalls
    (AbstractGameBoardInfo *gameboard,
     int *colorIndex,
     int totalCount,
     QPainter *painter,
     double xRate,
     double yRate,
     int frame,
     int size)
{
  if (ballsPixmaps.isEmpty())
    initBallsPixmaps();

  // Size of the ball
  double ballSize = gameboard->ballR() * 2;

  // Calulate some values used to locate the balls
  double scale = 0.12 * size + 0.28;
  QPointF originalCenterPos = gameboard->centerPos();
  QPointF currentCenterPos =
      QPointF(gameboard->width() * 0.2,
              gameboard->height() * 0.2);
  currentCenterPos.setX(
      (currentCenterPos.x() * (6 - size) +
       originalCenterPos.x() * (size - 1)) /
      5 * xRate);
  currentCenterPos.setY(
      (currentCenterPos.y() * (6 - size) +
       originalCenterPos.y() * (size - 1)) /
      5 * yRate);

  // For each ball
  for (int i = 0;i < totalCount;++i)
  {
    // If the ball exists
    if (colorIndex[i])
    {
      // Get the color
      int color = colorIndex[i] == -1 ? 6 : colorIndex[i];
      // Get the pixmap
      const QPixmap& p = ballsPixmaps[color]
          [frame % ballsFrameCounts[color]];
      // Calculate the position of the ball
      QPointF pos = gameboard->positionOfIndex(i);
      double dx =(pos.x() - originalCenterPos.x()) *
                 scale * xRate;
      double dy = (pos.y() - originalCenterPos.y()) *
                  scale * yRate;
      pos.setX(currentCenterPos.x() + dx);
      pos.setY(currentCenterPos.y() + dy);
      // Draw the pixmap
      drawPixmapAt(painter,
                   p,
                   ballSize / p.width() * scale * xRate *
                   1.1,
                   ballSize / p.height() * scale * yRate*
                   1.1,
                   pos,
                   true,
                   true);
    }
  }
}

void BasicPainter::paintItems
    (QPainter *painter,
     QVector<AbstractItem *> items,
     int width,
     int height,
     int frame)
{
  // Paint each item
  for (int i = 0;i < items.size();++i)
    items[i]->paint(painter, width, height, frame);
}

void BasicPainter::darken(QPainter *painter,
                          int width,
                          int height)
{
  painter->setPen(QColor(255,255,255,150));
  painter->setBrush(QBrush(QColor(255,255,255,150)));
  painter->fillRect(0,
                    0,
                    width,
                    height, Qt::Dense4Pattern);
}
