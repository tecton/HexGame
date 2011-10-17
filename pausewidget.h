#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>

// Forward declaration
class AbstractItem;
class StringItem;

/**
 * @brief A class to show the pause widget.
 */
class PauseWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  PauseWidget();

  /**
   * @brief Destructor.
   */
  ~PauseWidget();

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
  StringItem *pauseHint;

  // A vector stores the items,
  // used to paint and release the space
  QVector<AbstractItem *> myItems;

signals:
  /**
   * @brief A signal to tell other widget to resume.
   */
  void resume();
};

#endif // PAUSEWIDGET_H
