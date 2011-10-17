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

/**
 * @brief A class to do the effect paint.
 */
class EffectPainter
{
public:
  /**
   * @brief Constructor with the infomation of gameboard.
   */
  EffectPainter(AbstractGameBoardInfo *theGameboardInfo);

  /**
   * @brief Function to show that the user selected a ball.
   */
  void selectAt(int index);

  /**
   * @brief Function to clear the effect which shows that the user selected a
   * ball.
   */
  void clearSelectionHints();

  /**
   * @brief Function to show the elimintation if user confirm the gesture.
   */
  void bonusEliminationHintAt(int index);

  /**
   * @brief Function to clear the effect which shows the elimintation if user
   * confirm the gesture.
   */
  void clearBonusEliminationHints();

  /**
   * @brief Function to show the elimintation if user confirm the gesture.
   */
  void userMovingEliminationHintAt(int index);

  /**
   * @brief Function to clear the effect which shows the elimintation if user
   * confirm the gesture.
   */
  void clearUserMovingEliminationHints();

  /**
   * @brief Explode at the position.
   */
  void explodeAt(int index);

  /**
   * @brief Lighning at the position.
   */
  void lightningAt(int index/*, QVector<int> directions*/);

  /**
   * @brief Highlight at the position.
   */
  void highlightAt(int index);

  /**
   * @brief Show words at the position.
   */
  void wordsAt(QPointF pos, QString str, double size);

  /**
   * @brief Flash the whole screen
   */
  void flash();

  /**
   * @brief Show a hint at the position.
   */
  void hintAt(QPointF pos, bool rotate);


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

  // 4 lists of different effects
  QList<AbstractAgingEffect *> agingEffects;
  QList<AbstractLongLastingEffect *> selectionEffects;
  QList<AbstractLongLastingEffect *> bonusEliminateEffects;
  QList<AbstractLongLastingEffect *> userMovingEliminateEffects;
};

#endif // EFFECTPAINTER_H
