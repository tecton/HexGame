#ifndef PIXMAPOPERATIONS_H
#define PIXMAPOPERATIONS_H

#include <QPixmap>
#include <QVector>

class QPainter;

void initPixmaps(QString path,
                 QVector<QPixmap>& pixmaps,
                 int& frameCounts);

void initPixmaps(int count,
                 const char **paths,
                 QVector<QVector<QPixmap> >& pixmaps,
                 QVector<int>& frameCounts);

void drawPixmapAt(QPainter *painter,
                  const QPixmap& pixmap,
                  double xRate,
                  double yRate,
                  QPointF pos,
                  bool resize = false,
                  bool center = false);

#endif // PIXMAPOPERATIONS_H
