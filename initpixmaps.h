#ifndef INITPIXMAPS_H
#define INITPIXMAPS_H

#include <QPixmap>
#include <QVector>

void initPixmaps(QString path,
                 QVector<QPixmap>& pixmaps,
                 int& frameCounts);

void initPixmaps(int count,
                 const char **paths,
                 QVector<QVector<QPixmap> >& pixmaps,
                 QVector<int>& frameCounts);

#endif // INITPIXMAPS_H
