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
{":/images/balls/red0*.png",
 ":/images/balls/blue0*.png",
 ":/images/balls/green0*.png",
 ":/images/balls/yellow0*.png",
 ":/images/balls/purple0*.png",
 ":/images/balls/white0*.png",
 ":/images/balls/purple0*.png",
 ":/images/balls/white0*.png"};

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
// File path of the balls
// (The last two are the same as two before them, may make
//  cause some bug later)
const static char * kSmallBallsColorPaths[] =
{":/images/balls/red_small*.png",
 ":/images/balls/blue_small*.png",
 ":/images/balls/green_small*.png",
 ":/images/balls/yellow_small*.png",
 ":/images/balls/purple_small*.png",
 ":/images/balls/white_small*.png",
 ":/images/balls/purple_small*.png",
 ":/images/balls/white_small*.png"};

// Pixmaps of the balls
QVector<QVector<QPixmap> > smallBallsPixmaps;

// Total frame count of the balls
QVector<int> smallBallsFrameCounts;
void initSmallBallsPixmaps()
{
  initPixmaps(kBallsTotalColors,
              kSmallBallsColorPaths,
              smallBallsPixmaps,
              smallBallsFrameCounts);
}

// Total background number
const static int kBackgroundTotalColors = 9;
const static char * kBackgroundColorPaths[] =
{":/images/backgrounds/mainmenubackground.png",
 ":/images/backgrounds/puzzlemenubackground.png",
 ":/images/backgrounds/mainmenubackground.png",
 ":/images/backgrounds/37gamebackground.png",
 ":/images/backgrounds/61gamebackground.png",
 ":/images/backgrounds/helpbackgroundwithbutton.png",
 ":/images/backgrounds/achievementbackground.png",
 ":/images/backgrounds/37twoplayergamebackground.png",
 ":/images/backgrounds/mainselectplayersbackground.png"};

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

// File path of the balls
// (The last two are the same as two before them, may make
//  cause some bug later)
const static char * kLeftBallsColorPaths[] =
{":/images/balls/red_left*.png",
 ":/images/balls/blue_left*.png",
 ":/images/balls/green_left*.png",
 ":/images/balls/yellow_left*.png",
 ":/images/balls/purple_left*.png",
 ":/images/balls/white_left*.png",
 ":/images/balls/purple_left*.png",
 ":/images/balls/white_left*.png"};

// Pixmaps of the balls
QVector<QVector<QPixmap> > leftBallsPixmaps;

// Total frame count of the balls
QVector<int> leftBallsFrameCounts;
void initLeftBallsPixmaps()
{
  initPixmaps(kBallsTotalColors,
              kLeftBallsColorPaths,
              leftBallsPixmaps,
              leftBallsFrameCounts);
}

// File path of the balls
// (The last two are the same as two before them, may make
//  cause some bug later)
const static char * kRightBallsColorPaths[] =
{":/images/balls/red_right*.png",
 ":/images/balls/blue_right*.png",
 ":/images/balls/green_right*.png",
 ":/images/balls/yellow_right*.png",
 ":/images/balls/purple_right*.png",
 ":/images/balls/white_right*.png",
 ":/images/balls/purple_right*.png",
 ":/images/balls/white_right*.png"};

// Pixmaps of the balls
QVector<QVector<QPixmap> > rightBallsPixmaps;

// Total frame count of the balls
QVector<int> rightBallsFrameCounts;
void initRightBallsPixmaps()
{
  initPixmaps(kBallsTotalColors,
              kRightBallsColorPaths,
              rightBallsPixmaps,
              rightBallsFrameCounts);
}

void BasicPainter::paintBasicBalls(Ball **balls,
                                   int totalCount,
                                   QPainter *painter,
                                   double xRate,
                                   double yRate,
                                   int frame,
                                   QPointF (*positionTranslater)(QPointF),
                                   bool clockwise)
{
  // Init balls if neccessary
  if (leftBallsPixmaps.isEmpty())
    initLeftBallsPixmaps();

  // Init balls if neccessary
  if (rightBallsPixmaps.isEmpty())
    initRightBallsPixmaps();

  // For each ball
  for (int i = 0;i < totalCount;++i)
  {
    // If the ball exists
    if (balls[i])
    {
      // Get the color
      int colorIndex = balls[i]->getColor();
      // Get the pixmap
      const QPixmap& p = clockwise ?
                         leftBallsPixmaps[colorIndex][frame % leftBallsFrameCounts[colorIndex]] :
                         rightBallsPixmaps[colorIndex][frame % rightBallsFrameCounts[colorIndex]];
      // Get the position of the ball
      QPointF pos = positionTranslater(balls[i]->pos());
      // Reset the position according to the rate
      pos.setX(pos.x() * xRate);
      pos.setY(pos.y() * yRate);

      // Draw the pixmap
      drawPixmapAt(painter,
                   p,
                   xRate,
                   yRate,
                   pos,
                   true,
                   true);
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
  if (smallBallsPixmaps.isEmpty())
    initSmallBallsPixmaps();

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
      const QPixmap& p = smallBallsPixmaps[color]
          [frame % smallBallsFrameCounts[color]];
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
