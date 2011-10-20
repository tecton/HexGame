#ifndef TWOPLAYERMAINMENUWIDGET_H
#define TWOPLAYERMAINMENUWIDGET_H

#include "abstractpixmapwidget.h"

// File must include
#include <QVector>

// Forward declaration
class QTimer;
class AbstractItem;

/**
 * @brief A class of main menu.
 */
class TwoPlayerMainMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  TwoPlayerMainMenuWidget();

  /**
   * @brief Destructor.
   */
  ~TwoPlayerMainMenuWidget();

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
  QTimer *t;

  // Items of the game
  AbstractItem *helpItem;
  AbstractItem *achievementItem;
  AbstractItem *exitItem;
  AbstractItem *swapTimingItem;
  AbstractItem *rotateTimingItem;
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

#endif // TWOPLAYERMAINMENUWIDGET_H
