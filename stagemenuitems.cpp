
#include "stagemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

#define PREFIX     ":/images/stageitems/stage_img"
#define SUFFIX     "*.png"
#define ADVANCE    ":/images/stageitems/button_advance*.png"
#define NORMAL     ":/images/stageitems/button_normal*.png"
#define EXIT       ":/images/stageitems/button_exit*.png"

const char* names[] = {"01","02","03","04","05","06","07","08","09","10"};

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

StageMenuItem::StageMenuItem() :
    lock(false)
{
  initPixmaps(EXIT, pixmaps, count);
  setPos(QPointF(0, 0));
}

StageMenuItem::StageMenuItem(bool advanced) :
    lock(false)
{
  initPixmaps(advanced ? ADVANCE : NORMAL, pixmaps, count);
  setPos(QPointF(0, 0));
}

StageMenuItem::StageMenuItem(int stage, bool locked, bool advanced) :
    lock(locked)
{
  QString path = QObject::tr("%1%2%3%4%5").
                 arg(PREFIX).
                 arg(advanced?"_advanced":"").
                 arg(locked?"_locked":"").
                 arg(names[stage]).
                 arg(SUFFIX);
  initPixmaps(path, pixmaps, count);
  setPos(QPointF(0, 0));
}

void StageMenuItem::paint(QPainter *painter,
                          int width,
                          int height,
                          int frame)
{
  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  drawPixmapAt(painter,
               pixmaps[frame % count],
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

double StageMenuItem::r()
{
  return 100;
}
