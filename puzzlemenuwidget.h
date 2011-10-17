#ifndef PUZZLEMENUWIDGET_H
#define PUZZLEMENUWIDGET_H

#include "abstractpixmapwidget.h"

// File must include
#include <QVector>

// Forward declaration
class QTimer;
class AbstractItem;
class AbstractPuzzleMenuItem;

/**
 * @brief A class of the menu in puzzle game.
 */
class PuzzleMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  PuzzleMenuWidget();

  /**
   * @brief Destructor.
   */
  ~PuzzleMenuWidget();

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
  // exchange puzzle game item shown on widget
  AbstractPuzzleMenuItem *exchangeItem;
  // unite puzzle game item shown on widget
  AbstractPuzzleMenuItem *uniteItem;
  // lock puzzle game item shown on widget
  AbstractPuzzleMenuItem *lockItem;
  // exit item shown on widget
  AbstractItem *exitItem;
  // vector to collect all items
  QVector <AbstractItem *> myItems;
  int frameCount;

private slots:
  void advance();
};

#endif // PUZZLEMENUWIDGET_H
