#ifndef RESETWIDGET_H
#define RESETWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>

// Forward declaration
class AbstractItem;

/**
 * @brief A class to show the reset widget.
 *
 * In fact, the name "ConfirmOrCancelWidget" is better.
 */
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

  virtual void makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
      int width,
      int height);
  virtual void makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
      int width,
      int height);
  virtual void addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
      int width,
      int height);
  virtual QPointF toScene(double xRate, double yRate);
  virtual void dealPressed(QPointF mousePos, Qt::MouseButton button);
  virtual void dealMoved(QPointF mousePos, Qt::MouseButton button);
  virtual void dealReleased(QPointF mousePos, Qt::MouseButton button);
  virtual void getForcus(){}
  virtual void loseForcus(){}

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
  /**
   * @brief Confirm.
   */
  void confirm();

  /**
   * @brief Cancel.
   */
  void cancel();
  //@}
};

#endif // RESETWIDGET_H
