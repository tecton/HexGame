#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <QObject>

#include "achievementitem.h"
#include "statistic.h"
#include <QVector>

const int kFlameGetStage[] =      {10, 100, 1000, -1};
const int kStarGetStage[] =       {10, 100, 1000, -1};
const int kRotateClassicStage[] = {500, 1000, 2000, -1};
const int kTimingStage[] =        {400, 700, 1000, -1};

class QPainter;

/**
 * @brief A class to manage achievements according to the statistic.
 */
class Achievements : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief Constructor.
   */
  Achievements();

  virtual void paint(QPainter *painter, int width, int height);

  /**
   * @brief Advance the object to realize some animations.
   */
  void advance();

  /**
   * @brief Get the pointer of the item of the achievement.
   */
  static AbstractAchievementItem *getAchievementItem
      (AbstractAchievementItem::ItemType type);

  /**
   * @brief Get the pointers of all the items.
   */
  static QVector<AbstractAchievementItem *> getAchievementItems();

  /**
   * @brief Get the level of the achievement.
   *
   * Some hints will be shown once the user reached a higher level.
   */
  static int getAchievementLevel(AbstractAchievementItem::ItemType type,
                                 int value = -1);

private:
  QVector <AbstractAchievementItem *> items;
  int count;

public slots:
  /**
   * @brief Called when the statistic changed, it will check the statistic and
   * give correct response.
   */
  void statisticChanged(Statistic::StatisticType type,
                        int lastValue,
                        int currentValue);
};

#endif // ACHIEVEMENTS_H
