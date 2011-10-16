#ifndef ACHIEVEMENTWIDGET_H
#define ACHIEVEMENTWIDGET_H

// File must include
#include "abstractpixmapwidget.h"
#include <QVector>

// Forward declaration
class QTimer;
class AbstractAchievementItem;
class AbstractItem;

/**
 * @brief A class to show all the achievements.
 */
class AchievementWidget : public AbstractPixmapWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  AchievementWidget();

  /**
   * @brief Destructor.
   */
  ~AchievementWidget();

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

  AbstractItem *exitItem;

  QVector<AbstractAchievementItem *> achievementItems;
  QVector<AbstractItem *> myItems;

  // A value records the item at the position which user press
  AbstractItem *itemAtPressPos;

  int activeAchievementIndex;

private slots:
  void advance();
};

#endif // ACHIEVEMENTWIDGET_H
