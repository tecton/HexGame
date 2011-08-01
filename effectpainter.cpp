#include "effectpainter.h"

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include "abstractgameboardinfo.h"

#define EFFECT_LAST_TIME  15

EffectPainter::EffectPainter(
    AbstractGameBoardInfo *theGameboardInfo) :
    gameboardInfo(theGameboardInfo)
{
}

void EffectPainter::eliminationHintAt(int index)
{
  EliminationHint hint;
  hint.age = 0;
  hint.pos = gameboardInfo->centerPositionOfIndex(index);
  eliminationHints.push_back(hint);
}

void EffectPainter::clearEliminationHints()
{
  eliminationHints.clear();
}

void EffectPainter::explodeAt(int index)
{
  ExplodeInfo info;
  info.age = 0;
  info.pos = gameboardInfo->centerPositionOfIndex(index);
  explodeInfos.push_back(info);
}

void EffectPainter::lightningAt(int index, QVector<int> directions)
{
  LightningInfo info;
  info.age = 0;
  info.pos = gameboardInfo->centerPositionOfIndex(index);
  info.directions = directions;
  lightningInfos.push_back(info);
}

void EffectPainter::highlightAt(int index)
{
  HighlightInfo info;
  info.age = 0;
  QPointF leftTop = gameboardInfo->positionOfIndex(index);
  double r = gameboardInfo->ballR();
  QPointF rightBottom = QPointF(leftTop.x() + 2 * r, leftTop.y() + 2 * r);
  QRectF rect = QRectF(leftTop, rightBottom);
  info.area = rect;
  highlightInfos.push_back(info);
}


void EffectPainter::highlightAll()
{
  HighlightInfo info;
  info.age = 0;
  QPointF leftTop = QPointF(0, 0);
  QPointF rightBottom = QPointF(gameboardInfo->width(),
                                gameboardInfo->height());
  QRectF rect = QRectF(leftTop, rightBottom);
  info.area = rect;
  highlightInfos.push_back(info);
}

void EffectPainter::clearEffects()
{
  eliminationHints.clear();
  explodeInfos.clear();
  lightningInfos.clear();
  highlightInfos.clear();
}

void EffectPainter::advance()
{
//  EliminationHint eliminationHint;
//  foreach (eliminationHint, eliminationHints)
//  {
//    ++eliminationHint.age;
//    if (explodeInfo.age > EFFECT_LAST_TIME)
//      eliminationHints.removeOne(eliminationHints);
//  }

  ExplodeInfo explodeInfo;
  foreach (explodeInfo, explodeInfos)
  {
    ++explodeInfo.age;
    if (explodeInfo.age > EFFECT_LAST_TIME)
      explodeInfos.removeOne(explodeInfo);
  }

  LightningInfo lightningInfo;
  foreach (lightningInfo, lightningInfos)
  {
    ++lightningInfo.age;
    if (lightningInfo.age > EFFECT_LAST_TIME)
      lightningInfos.removeOne(lightningInfo);
  }

  HighlightInfo highlightInfo;
  foreach (highlightInfo, highlightInfos)
  {
    ++highlightInfo.age;
    if (highlightInfo.age > EFFECT_LAST_TIME)
      highlightInfos.removeOne(highlightInfo);
  }
}

void EffectPainter::paint(QPainter *painter,
                          double xRate,
                          double yRate)
{
  // TODO:BLABLABLA
}
