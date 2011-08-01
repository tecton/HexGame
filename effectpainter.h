#ifndef EFFECTPAINTER_H
#define EFFECTPAINTER_H

#include <QVector>
#include <QList>
#include <QPointF>
#include <QRectF>

class QPainter;
class AbstractGameBoardInfo;

struct EliminationHint
{
  int age;
  QPointF pos;
  bool operator==(const EliminationHint& another)
  {
    return pos == another.pos && age == another.age;
  }
};

struct ExplodeInfo
{
  int age;
  QPointF pos;
  bool operator==(const ExplodeInfo& another)
  {
    return pos == another.pos && age == another.age;
  }
};

struct LightningInfo
{
  int age;
  QPointF pos;
  QVector<int> directions;
  bool operator==(const LightningInfo& another)
  {
    return pos == another.pos &&
           age == another.age &&
           directions == another.directions;
  }
};

struct HighlightInfo
{
  int age;
  QRectF area;
  bool operator==(const HighlightInfo& another)
  {
    return area == another.area && age == another.age;
  }
};

class EffectPainter
{
public:
  EffectPainter(AbstractGameBoardInfo *theGameboardInfo);

  void eliminationHintAt(int index);
  void clearEliminationHints();

  void explodeAt(int index);
  void lightningAt(int index, QVector<int> directions);
  void highlightAt(int index);
//  void highlightGameboard();
  void highlightAll();
  //BLABLABLA

  void clearEffects();

  void advance();

  void paint(QPainter *painter,
             double xRate,
             double yRate);

private:
  AbstractGameBoardInfo *gameboardInfo;

  QList<EliminationHint> eliminationHints;
  QList<ExplodeInfo> explodeInfos;
  QList<LightningInfo> lightningInfos;
  QList<HighlightInfo> highlightInfos;
};

#endif // EFFECTPAINTER_H
