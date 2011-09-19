/*
** A class to show the reset widget.
*/

#ifndef RESETWIDGET_H
#define RESETWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>

// Forward declaration
class AbstractItem;

class ResetWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  ResetWidget();

  /**
   * @brief Destructor.
   */
  ~ResetWidget();

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
  // Items of the game
  AbstractItem *confirmItem;
  AbstractItem *cancelItem;

  // A vector stores the items,
  // used to paint and release the space
  QVector<AbstractItem *> myItems;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos;

signals:
  //@{
  /** Two signals to show the result. */
  void confirm();
  void cancel();
  //@}
};

#endif // RESETWIDGET_H
