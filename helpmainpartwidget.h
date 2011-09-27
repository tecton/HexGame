#ifndef HELPMAINPARTWIDGET_H
#define HELPMAINPARTWIDGET_H

// File must include
#include <QPixmap>
#include <QPointF>
#include <QString>
#include <QVector>

class QPainter;

class HelpMainPartWidget
{
public:
  HelpMainPartWidget(QString backgroundPath,
                     QVector<QPointF> animCenters,
                     QVector<QString> animPaths);

  void paint(QPainter *painter, int width, int height, int frame);

private:
  int backgroundFrameCounts;
  QVector<QPixmap> backgroundPixmaps;
  QVector<QPointF> animCenterPoints;
  QVector<int> animFrameCounts;
  QVector<QVector<QPixmap> > animPixmaps;
};

#endif // HELPMAINPARTWIDGET_H
