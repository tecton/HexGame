/*
** Functions used to deal with pixmaps
*/

#ifndef PIXMAPOPERATIONS_H
#define PIXMAPOPERATIONS_H

// File must include
#include <QPixmap>
#include <QVector>

// Forward declaration
class QPainter;

/**
 * @brief Init pixmaps of a thing.
 */
void initPixmaps(QString path, QVector<QPixmap>& pixmaps, int& frameCounts);

/**
 * @brief Init pixmaps of some things.
 */
void initPixmaps(int count,
                 const char **paths,
                 QVector<QVector<QPixmap> >& pixmaps,
                 QVector<int>& frameCounts);

/**
 * @brief Draw a pixmap with some options.
 */
void drawPixmapAt(QPainter *painter,
                  const QPixmap& pixmap,
                  double xRate,
                  double yRate,
                  QPointF pos,
                  bool resize = false,
                  bool center = false);

#endif // PIXMAPOPERATIONS_H
