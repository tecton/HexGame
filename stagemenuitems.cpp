
#include "stagemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>

#include <QMessageBox>

StageMenuItem::StageMenuItem(QString path, int number)
{
  pixmaps.reserve(number);
  count = number;
  for (int i = 0; i < number; ++i)
  {
    QFileInfo fi(path);
    foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
    {
      // Get the pixmap of the path
      pixmaps << QPixmap(fi.path() + "/" + entry);
    }
  }
  setPos(QPointF(0, 0));
}

void StageMenuItem::paint(QPainter *painter,
                          int width,
                          int height,
                          int frame)
{
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawPixmap(QPointF(x, y), pixmaps[frame % count]);
  //  QMessageBox::critical(0,"","Painted one item");
}
