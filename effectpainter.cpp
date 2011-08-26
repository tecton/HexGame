#include "effectpainter.h"

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QPolygonF>
#include "abstractgameboardinfo.h"

#define EFFECT_LAST_TIME  15



class AbstractEffect
{
public:
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)=0;
};

class AbstractAgingEffect : public AbstractEffect
{
public:
  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)=0;

  inline bool advance()
  {
    ++age;
    return age <= ageLimit;
  }

  inline int getAge()
  {return age;}

  inline void setAge(int v)
  {age = v;}

  inline int getLimit()
  {return ageLimit;}

  inline int setLimit(int v)
  {ageLimit = v;}

private:
  int age;
  int ageLimit;
};

class EliminationHint : public AbstractLongLastingEffect
{
public:
  EliminationHint(QRectF position) :
      pos(position)
  {}

  inline void setPos(QRectF position)
  {pos = position;}

  inline QRectF getPos()
  {return pos;}

  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    painter->setBrush(QBrush(QColor(255,255,255,100)));
    painter->setPen(QPen(QColor(255,255,255,100)));

    QVector<QPointF> points;
    points.push_back(QPointF(xRate * pos.left(),
                             yRate * (pos.top() + pos.bottom()) / 2));
    points.push_back(QPointF(xRate * (3 * pos.left() + pos.right()) / 4,
                             yRate * pos.top()));
    points.push_back(QPointF(xRate * (pos.left() + 3 * pos.right()) / 4,
                             yRate * pos.top()));
    points.push_back(QPointF(xRate * pos.right(),
                             yRate * (pos.top() + pos.bottom()) / 2));
    points.push_back(QPointF(xRate * (pos.left() + 3 * pos.right()) / 4,
                             yRate * pos.bottom()));
    points.push_back(QPointF(xRate * (3 * pos.left() + pos.right()) / 4,
                             yRate * pos.bottom()));
    QPolygonF poly(points);

    painter->drawPolygon(poly);
    //BLABLABLA
  }

private:
  QRectF pos;
};

class ExplodeInfo : public AbstractAgingEffect
{
public:
  ExplodeInfo(QPointF position) :
      pos(position)
  {
    setAge(0);
    setLimit(20);
  }

  inline void setPos(QPointF position)
  {pos = position;}

  inline QPointF getPos()
  {return pos;}

  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    //BLABLABLA
  }

private:
  QPointF pos; // Center of the explosion
};

class LightningInfo : public AbstractAgingEffect
{
public:
  LightningInfo(QPointF position) :
      pos(position)
  {
    setAge(0);
    setLimit(20);
  }

  inline void setPos(QPointF position)
  {pos = position;}

  inline QPointF getPos()
  {return pos;}

  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    //BLABLABLA
  }

private:
  QPointF pos; // Center of the lightning
  // QVector<int> directions;
};

class HighlightInfo : public AbstractAgingEffect
{
public:
  HighlightInfo(QRectF position) :
      pos(position)
  {
    setAge(0);
    setLimit(20);
  }

  inline void setPos(QRectF position)
  {pos = position;}

  inline QRectF getPos()
  {return pos;}

  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    //BLABLABLA
  }

private:
  QRectF pos;
};

EffectPainter::EffectPainter(
    AbstractGameBoardInfo *theGameboardInfo) :
    gameboardInfo(theGameboardInfo)
{
}

void EffectPainter::bonusEliminationHintAt(int index)
{
  QPointF center = gameboardInfo->centerPositionOfIndex(index);
  QPointF leftTop = QPointF(center.x() - 0.75 * gameboardInfo->intervalBetweenTwoLayers(),
                            center.y() - 0.75 * 0.866 * gameboardInfo->intervalBetweenTwoLayers());
  QPointF rightBottom = QPointF(center.x() + 0.75 * gameboardInfo->intervalBetweenTwoLayers(),
                                center.y() + 0.75 * 0.866 * gameboardInfo->intervalBetweenTwoLayers());
  QRectF rect = QRectF(leftTop, rightBottom);
  EliminationHint *hint = new EliminationHint(rect);
  bonusEliminateEffects.push_back(hint);
}

void EffectPainter::clearBonusEliminationHints()
{
  // Currently, there are only one kind of long lasting effect
  for (int i = 0;i < bonusEliminateEffects.size();++i)
    delete bonusEliminateEffects[i];
  bonusEliminateEffects.clear();
}

void EffectPainter::userMovingEliminationHintAt(int index)
{
  QPointF center = gameboardInfo->centerPositionOfIndex(index);
  QPointF leftTop = QPointF(center.x() - 0.75 * gameboardInfo->intervalBetweenTwoLayers(),
                            center.y() - 0.75 * 0.866 * gameboardInfo->intervalBetweenTwoLayers());
  QPointF rightBottom = QPointF(center.x() + 0.75 * gameboardInfo->intervalBetweenTwoLayers(),
                                center.y() + 0.75 * 0.866 * gameboardInfo->intervalBetweenTwoLayers());
  QRectF rect = QRectF(leftTop, rightBottom);
  EliminationHint *hint = new EliminationHint(rect);
  userMovingEliminateEffects.push_back(hint);
}

void EffectPainter::clearUserMovingEliminationHints()
{
  // Currently, there are only one kind of long lasting effect
  for (int i = 0;i < userMovingEliminateEffects.size();++i)
    delete userMovingEliminateEffects[i];
  userMovingEliminateEffects.clear();
}

void EffectPainter::explodeAt(int index)
{
  ExplodeInfo *info = new ExplodeInfo(gameboardInfo->centerPositionOfIndex(index));
  agingEffects.push_back(info);
}

void EffectPainter::lightningAt(int index/*, QVector<int> directions*/)
{
  LightningInfo *info = new LightningInfo(gameboardInfo->centerPositionOfIndex(index));
  agingEffects.push_back(info);
}

void EffectPainter::highlightAt(int index)
{
  QPointF center = gameboardInfo->centerPositionOfIndex(index);
  QPointF leftTop = QPointF(center.x() - gameboardInfo->intervalBetweenTwoLayers(),
                            center.y() - 0.866 * gameboardInfo->intervalBetweenTwoLayers());
  QPointF rightBottom = QPointF(center.x() + gameboardInfo->intervalBetweenTwoLayers(),
                                center.y() + 0.866 * gameboardInfo->intervalBetweenTwoLayers());
  QRectF rect = QRectF(leftTop, rightBottom);
  HighlightInfo *info = new HighlightInfo(rect);
  agingEffects.push_back(info);
}


//void EffectPainter::highlightAll()
//{
//  HighlightInfo info;
//  info.age = 0;
//  QPointF leftTop = QPointF(0, 0);
//  QPointF rightBottom = QPointF(gameboardInfo->width(),
//                                gameboardInfo->height());
//  QRectF rect = QRectF(leftTop, rightBottom);
//  info.area = rect;
//  highlightInfos.push_back(info);
//}

void EffectPainter::clearEffects()
{
  clearBonusEliminationHints();
  clearUserMovingEliminationHints();
  for (int i = 0;i < agingEffects.size();++i)
    delete agingEffects[i];
  agingEffects.clear();
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

  AbstractAgingEffect *agingEffect;
  foreach (agingEffect, agingEffects)
  {
    if (!agingEffect->advance())
    {
      delete agingEffect;
      agingEffects.removeOne(agingEffect);
    }
  }
}

void EffectPainter::paint(QPainter *painter,
                          double xRate,
                          double yRate)
{
  AbstractEffect *effect;
  foreach (effect, agingEffects)
   effect->paint(gameboardInfo, painter, xRate, yRate);

  foreach (effect, bonusEliminateEffects)
   effect->paint(gameboardInfo, painter, xRate, yRate);

  foreach (effect, userMovingEliminateEffects)
   effect->paint(gameboardInfo, painter, xRate, yRate);
  // TODO:BLABLABLA
}
