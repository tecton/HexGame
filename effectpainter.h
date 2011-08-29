#ifndef EFFECTPAINTER_H
#define EFFECTPAINTER_H

//#include <QVector>
#include <QList>
#include <QPointF>
#include <QRectF>

class QPainter;
class AbstractGameBoardInfo;

class AbstractEffect;
class AbstractAgingEffect;
typedef AbstractEffect AbstractLongLastingEffect;

class EffectPainter
{
public:
  EffectPainter(AbstractGameBoardInfo *theGameboardInfo);

  void bonusEliminationHintAt(int index);
  void clearBonusEliminationHints();
  void userMovingEliminationHintAt(int index);
  void clearUserMovingEliminationHints();

  void explodeAt(int index);
  void lightningAt(int index/*, QVector<int> directions*/);
  void highlightAt(int index);

  void wordsAt(QPointF pos, QString str, double size);

  void flash();

  void hintAt(QPointF pos, bool rotate);

//  void highlightGameboard();
//  void highlightAll();
  //BLABLABLA

  void clearEffects();

  void advance();

  void paint(QPainter *painter,
             double xRate,
             double yRate);

private:
  AbstractGameBoardInfo *gameboardInfo;

  QList<AbstractAgingEffect *> agingEffects;
  QList<AbstractLongLastingEffect *> bonusEliminateEffects;
  QList<AbstractLongLastingEffect *> userMovingEliminateEffects;
};

#endif // EFFECTPAINTER_H
