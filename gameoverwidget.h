/*
** A class to show the game over widget.
** May be seperated later.
*/

#ifndef GAMEOVERWIDGET_H
#define GAMEOVERWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>

// Forward declaration
class AbstractItem;
class IntegerItem;
class StringItem;

class GameOverWidget : public AbstractPixmapWidget
{
public:
  /**
   * @brief Constructor with the index of the game and the score of the game.
   */
  GameOverWidget(int gameIndex, int score);

  /**
   * @brief Destructor.
   */
  ~GameOverWidget();

  //@{
  /** Functions most overloaded. */
  virtual void makePixmap(QPixmap& pixmap, int width, int height);
  virtual void makeBasicPixmap(QPixmap& pixmap, int width, int height);
  virtual void addEffect(QPixmap& pixmap, int width, int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus(){}
  //@}

private:
  // The index of the game
  int index;

  // Items of the game
  IntegerItem *scoreHint;
  StringItem *newGameHint;
  AbstractItem *confirmItem;
  AbstractItem *cancelItem;

  // A vector stores the items,
  // used to paint and release the space
  QVector<AbstractItem *> myItems;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos;
};

#endif // GAMEOVERWIDGET_H
