#ifndef HELPWIDGET2_H
#define HELPWIDGET2_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>
#include <QPointF>
#include <QPixmap>

// Forward declaration
class AbstractItem;
class HelpItem;
class HelpMainPartWidget;

/**
 * @brief A class of a help widget which shows the help.
 */
class HelpWidget2 : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  HelpWidget2();

  /**
   * @brief Destructor.
   */
  ~HelpWidget2();

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
  HelpItem **helpThemeItems;

  AbstractItem *exitItem;

  HelpMainPartWidget **helpThemeWidgets;

  // A vector stores the items,
  // used to paint and release the space
  QVector<AbstractItem *> myItems;

  // A value records the item at the
  // position which user press
  AbstractItem *itemAtPressPos;

  int currentItemIndex;

  int mainPartFrameCount;
};

#endif // HELPWIDGET2_H
