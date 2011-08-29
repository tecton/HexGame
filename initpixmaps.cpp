#include "initpixmaps.h"

#include <QFileInfo>
#include <QDir>

void initPixmaps(QString path,
                 QVector<QPixmap>& pixmaps,
                 int& frameCounts)
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
