#include "effectpainter.h"

#include <QPainter>
#include <QPointF>
#include <QRectF>
#include <QPolygonF>
#include <QRadialGradient>
#include <QLinearGradient>
#include "abstractgameboardinfo.h"
#include "gamemath.h"

#define EFFECT_LAST_TIME  15

double kkx[3] = {-0.5, 0.5, 1};
double kky[3] = {-0.866, -0.866, 0};

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

  inline void setLimit(int v)
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
    setLimit(5);
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
    double r = 3 * theGameboardInfo->intervalBetweenTwoLayers() *
               getAge() /
               getLimit();
    QPointF pos2 = scale(pos, xRate, yRate);
    QRadialGradient gradient = QRadialGradient(pos2, r, pos2);
    gradient.setColorAt(0, QColor(255,255,255,200));
    gradient.setColorAt(1, QColor(255,255,255,50));
    painter->setBrush(QBrush(gradient));

    painter->drawEllipse(pos2, r, r);
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
    linearPoints[0] = QPointF(pos.x() - 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[1] = QPointF(pos.x() + 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[2] = QPointF(pos.x(), pos.y() + 15);
    setAge(0);
    setLimit(5);
  }

  inline void setPos(QPointF position)
  {
    pos = position;
    linearPoints[0] = QPointF(pos.x() - 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[1] = QPointF(pos.x() + 7.5 * 1.732, pos.y() + 7.5);
    linearPoints[2] = QPointF(pos.x(), pos.y() + 15);

  }

  inline QPointF getPos()
  {return pos;}

  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    QPointF pos2 = scale(pos, xRate, yRate);
    double r = distanceOfTwoPoints(QPointF(0, 0),
                                   QPointF(theGameboardInfo->width(), theGameboardInfo->height())) *
               getAge() /
               getLimit() /
               2;
    for (int i = 0;i < 3;++i)
    {
      QLinearGradient gradient = QLinearGradient(pos2, scale(linearPoints[i], xRate, yRate));
      gradient.setColorAt(0, QColor(255,255,255,255));
      gradient.setColorAt(1, QColor(255,255,255,100));
      gradient.setSpread(QGradient::ReflectSpread);
      QPen pen = QPen(QBrush(gradient), 30);
      painter->setPen(pen);
      QPointF posA =QPointF(pos2.x() + kkx[i] * r, pos2.y() + kky[i] * r);
      QPointF posB =QPointF(pos2.x() - kkx[i] * r, pos2.y() - kky[i] * r);
      painter->drawLine(posA, posB);
    }
  }

private:
  QPointF pos; // Center of the lightning
  QPointF linearPoints[3];
  // QVector<int> directions;
};

class HighlightInfo : public AbstractAgingEffect
{
public:
  HighlightInfo(QPointF position) :
      pos(position)
  {
    setAge(0);
    setLimit(10);
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
    QColor color = QColor(255,255,255,100 + 50 * getAge() / getLimit());
//    QPointF posTL = pos.topLeft();
//    QPointF posBR = pos.bottomRight();
//    QPointF center = pos.center();
//    posTL = scale(posTL, xRate, yRate);
//    posBR = scale(posBR, xRate, yRate);
    QPointF pos2 = scale(pos, xRate, yRate);

    double r = theGameboardInfo->intervalBetweenTwoLayers() *
               getAge() /
               getLimit();
    QRadialGradient gradient = QRadialGradient(pos2, r, pos2);
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, QColor(255, 255, 255, 50));

    painter->setPen(QColor(255, 255, 255, 0));
    painter->setBrush(QBrush(gradient));
    painter->drawEllipse(pos2, r, r);
  }

private:
  QPointF pos;
};

class WordsInfo : public AbstractAgingEffect
{
public:
  WordsInfo(QString str, QPointF position, double size) :
      string(str),
      pos(position)
  {
    f.setPointSize(size * 8);
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
    QPointF pos2 = pos;
    pos2.setY(pos2.y() - getAge());
    pos2 = scale(pos2, xRate, yRate);
    painter->setPen(QColor(255, 255, 255, 100 + 50 * getAge() / getLimit()));
    painter->setFont(f);
    painter->drawText(pos2, string);
  }

private:
  QString string;
  QPointF pos;
  QFont f;
};

class FlashInfo : public AbstractAgingEffect
{
public:
  FlashInfo()
  {
    setAge(0);
    setLimit(15);
  }

  virtual void paint(AbstractGameBoardInfo *theGameboardInfo,
                     QPainter *painter,
                     double xRate,
                     double yRate)
  {
    QColor color(255, 255, 255, getAge() % 5 * 10);
    painter->setPen(color);
    painter->setBrush(color);
    painter->drawRect(0,
                      0,
                      theGameboardInfo->width() * xRate,
                      theGameboardInfo->height() * yRate);
  }
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
//  QPointF leftTop = QPointF(center.x() - gameboardInfo->intervalBetweenTwoLayers(),
//                            center.y() - 0.866 * gameboardInfo->intervalBetweenTwoLayers());
//  QPointF rightBottom = QPointF(center.x() + gameboardInfo->intervalBetweenTwoLayers(),
//                                center.y() + 0.866 * gameboardInfo->intervalBetweenTwoLayers());
//  QRectF rect = QRectF(leftTop, rightBottom);
  HighlightInfo *info = new HighlightInfo(center);
  agingEffects.push_back(info);
}

void EffectPainter::wordsAt(QPointF pos, QString str, double size)
{
  WordsInfo *info = new WordsInfo(str, pos, size);
  agingEffects.push_back(info);
}

void EffectPainter::flash()
{
  FlashInfo *info = new FlashInfo();
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
