#ifndef ACHIEVEMENTITEM_H
#define ACHIEVEMENTITEM_H

#include "abstractitem.h"

#include <QPixmap>
#include <QPointF>
#include <QString>
#include <QVector>

/**
 * @brief An abstract class of items used in achievement widget.
 */
class AbstractAchievementItem : public AbstractItem
{
public:
  /**
   * @brief Type of the items.
   */
  enum ItemType{FlameGet=0,
                StarGet,
                RotateClassic,
                Timing,
                RotatePuzzle};

  virtual void paint(QPainter *painter, int width, int height, int frame)=0;

  /**
   * @brief Paint the description of the achievement in the given rect.
   */
  virtual void paintDescription(QPainter *painter, QRectF rect, int frame)=0;

  bool in(QPointF mousePos);
};

/**
 * @brief A class of items used in achievement widget to show number of flames
 * the user has got.
 */
class FlameGetItem : public AbstractAchievementItem
{
public:
  FlameGetItem(int theLevel, int theCurrent);
  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual void paintDescription(QPainter *painter, QRectF rect, int frame);

private:
  int level;
  int current;
};






#endif // ACHIEVEMENTITEM_H
