#include "helpmainpartwidget.h"

#include <QPainter>
#include "pixmapoperations.h"
#include "gamemath.h"

#define FADE_IN_FRAMES  10

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
  if (frame < FADE_IN_FRAMES)
    painter->setOpacity(1.0 * frame / FADE_IN_FRAMES);

  double xRate;
  double yRate;
  {
    const QPixmap& p = backgroundPixmaps[frame % backgroundFrameCounts];
    xRate = 1.0 * width / p.width();
    yRate = 1.0 * height / p.height();
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

  painter->setOpacity(1);
}
