#include "helpmainpartwidget.h"

#include <QPainter>
#include "pixmapoperations.h"
#include "gamemath.h"

HelpMainPartWidget::HelpMainPartWidget(QString backgroundPath,
                                       QVector<QPointF> animCenters,
                                       QVector<QString> animPaths) :
    animCenterPoints(animCenters)
{
  initPixmaps(animPaths, animPixmaps, animFrameCounts);
  initPixmaps(backgroundPath, backgroundPixmaps, backgroundFrameCounts);
}

void HelpMainPartWidget::paint(QPainter *painter,
                               int width,
                               int height,
                               int frame)
{
  double xRate;
  double yRate;
  {
    const QPixmap& p = backgroundPixmaps[frame % backgroundFrameCounts];
    xRate = 1.0 * p.width() / width;
    yRate = 1.0 * p.height() / height;
    drawPixmapAt(painter,
                 p,
                 xRate,
                 yRate,
                 QPointF(width / 2.0, height / 2.0),
                 true,
                 true);
  }

  for (int i = 0;i < animCenterPoints.size();++i)
  {
    const QPixmap& p = animPixmaps[i][frame % animFrameCounts[i]];
    QPointF pos = animCenterPoints[i];
    pos = scale(pos, xRate, yRate);
    drawPixmapAt(painter,
                 p,
                 xRate,
                 yRate,
                 pos,
                 true,
                 true);
  }
}
