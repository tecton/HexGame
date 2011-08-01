#include "basicpainter.h"


#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QPen>

#include "abstractitem.h"
#include "ball.h"

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
  pixmaps.reserve(kTotalColors);
  frameCounts.reserve(kTotalColors);
  for (int i = 0;i < kTotalColors;++i)
  {
    int count = 0;
    QVector<QPixmap> current;
    QString path(kColorPaths[i]);
    QFileInfo fi(path);
    // For each file of the color
    foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
    {
      // Get the pixmap of the path
      current << QPixmap(fi.path() + "/" + entry);
      ++count;
    }
    pixmaps << current;
    frameCounts << count;
  }
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
      painter->drawPixmap(
          pos,
          pixmaps[colorIndex][frame % frameCounts[colorIndex]]);
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
