#include "pixmapoperations.h"

#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "config.h"

void initPixmaps(QString path, QVector<QPixmap>& pixmaps, int& frameCounts)
{
  frameCounts = 0;
  QFileInfo fi(path);
  frameCounts = QDir(fi.path(), fi.fileName()).entryList().size();
  pixmaps.reserve(frameCounts);
  foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
  {
    // Get the pixmap of the path
    pixmaps << QPixmap(fi.path() + "/" + entry);
  }
}

void initPixmaps(int count,
                 const char **paths,
                 QVector<QVector<QPixmap> >& pixmaps,
                 QVector<int>& frameCounts)
{
  pixmaps.clear();
  pixmaps.reserve(count);
  frameCounts.clear();
  frameCounts.reserve(count);
  for (int i = 0;i < count;++i)
  {
    QVector<QPixmap> current;
    QString path(paths[i]);
    QFileInfo fi(path);
    int count = QDir(fi.path(), fi.fileName()).entryList().size();
    current.reserve(count);
    // For each file of the color
    foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
    {
      // Get the pixmap of the path
      current << QPixmap(fi.path() + "/" + entry);
    }
    pixmaps << current;
    frameCounts << count;
  }
}

void initPixmaps(QVector<QString> paths,
                 QVector<QVector<QPixmap> >& pixmaps,
                 QVector<int>& frameCounts)
{
  int count = paths.size();
  pixmaps.clear();
  pixmaps.reserve(count);
  frameCounts.clear();
  frameCounts.reserve(count);
  for (int i = 0;i < count;++i)
  {
    QVector<QPixmap> current;
    QString path(paths[i]);
    QFileInfo fi(path);
    int count = QDir(fi.path(), fi.fileName()).entryList().size();
    current.reserve(count);
    // For each file of the color
    foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
    {
      // Get the pixmap of the path
      current << QPixmap(fi.path() + "/" + entry);
    }
    pixmaps << current;
    frameCounts << count;
  }
}

void drawPixmapAt(QPainter *painter,
                  const QPixmap& pixmap,
                  double xRate,
                  double yRate,
                  QPointF pos,
                  bool resize,
                  bool center)
{
  // Calculate the width and height the pixmap should be
  double width = pixmap.width();
  double height = pixmap.height();

  if (xRate >= 0.999 &&
      xRate <= 1.001 &&
      yRate >= 0.999 &&
      yRate <= 1.001)
    resize = false;

  if (resize)
  {
    width *= xRate;
    height *= yRate;
  }

  // Calculate the left up position of the pixmap
  QPointF leftUp = pos;
  if (center)
  {
    leftUp.setX(leftUp.x() - width / 2);
    leftUp.setY(leftUp.y() - height / 2);
  }
#ifdef USE_TRANSLATE_AND_SCALE
  painter->translate(leftUp.x(), leftUp.y());
  painter->scale(xRate, yRate);

  // Draw the pixmap
  painter->drawPixmap(0, 0, pixmap);

  painter->scale(1.0 / xRate, 1.0 / yRate);
  painter->translate(-leftUp.x(), -leftUp.y());
#else
  painter->drawPixmap(leftUp.x(), leftUp.y(), width, height, pixmap);
#endif
}
