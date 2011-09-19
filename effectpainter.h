/*
** A class to do the effect paint.
*/

#ifndef EFFECTPAINTER_H
#define EFFECTPAINTER_H

// File must include
#include <QList>
#include <QPointF>
#include <QRectF>

// Forward declaration
class QPainter;
class AbstractGameBoardInfo;
class AbstractEffect;
class AbstractAgingEffect;

typedef AbstractEffect AbstractLongLastingEffect;

class EffectPainter
{
public:
  /**
   * @brief Constructor with the infomation of gameboard.
   */
  EffectPainter(AbstractGameBoardInfo *theGameboardInfo);

  //@{
  /** 2 functions related to user's gesture. */
  void selectAt(int index);
  void clearSelectionHints();
  //@}

  //@{
  /** 4 functions related to elimination hint. */
  void bonusEliminationHintAt(int index);
  void clearBonusEliminationHints();
  void userMovingEliminationHintAt(int index);
  void clearUserMovingEliminationHints();
  //@}

  //@{
  /** 6 Effects. */
  void explodeAt(int index);
  void lightningAt(int index/*, QVector<int> directions*/);
  void highlightAt(int index);
  void wordsAt(QPointF pos, QString str, double size);
  void flash();
  void hintAt(QPointF pos, bool rotate);
  //@}

//  void highlightGameboard();
//  void highlightAll();
  //BLABLABLA

  /**
   * @brief Clear effects.
   */
  void clearEffects();

  /**
   * @brief Advance.
   */
  void advance();

  /**
   * @brief Paint the effects.
   */
  void paint(QPainter *painter, double xRate, double yRate);

private:
  // Infomation of the gameboard
  AbstractGameBoardInfo *gameboardInfo;

  // 3 lists of different effects
  QList<AbstractAgingEffect *> agingEffects;
  QList<AbstractLongLastingEffect *> selectionEffects;
  QList<AbstractLongLastingEffect *> bonusEliminateEffects;
  QList<AbstractLongLastingEffect *> userMovingEliminateEffects;
};

#endif // EFFECTPAINTER_H
