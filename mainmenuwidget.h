#ifndef MAINMENUWIDGET_H
#define MAINMENUWIDGET_H

#include "abstractpixmapwidget.h"

// File must include
#include <QVector>

// Forward declaration
//class QTimer;
class AbstractItem;
class MainMenuGameItem;

class MainMenuWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  MainMenuWidget();

  /**
   * @brief Destructor.
   */
  ~MainMenuWidget();

  //@{
  /** Functions most overloaded. */
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
  //@}

private:
  // The timer(currently not used)
//  QTimer *t;

  // Items of the game
  MainMenuGameItem *items[9];
  QVector <AbstractItem *> myItems;
//  int frameCount;

private slots:
//  void advance();
};

#endif // MAINMENUWIDGET_H
