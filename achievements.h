#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <QObject>

#include "achievementitem.h"
#include "statistic.h"
#include <QVector>

static const int kFlameGetStage[] =      {10, 100, 1000, -1};
static const char *kFlameGetTitle[] =
{
"Flame Getter",
"Flame Getter 1",
"Flame Getter 2",
"Flame Getter 3"
};
static const char *kFlameGetPaths[] =
{
":/images/achievementitems/none.png",
":/images/achievementitems/flamebronze.png",
":/images/achievementitems/flamesilver.png",
":/images/achievementitems/flamegold.png"
};
static const int kStarGetStage[] =       {10, 100, 1000, -1};
static const char *kStarGetTitle[] =
{
"Star Getter",
"Star Getter 1",
"Star Getter 2",
"Star Getter 3"
};
static const char *kStarGetPaths[] =
{
":/images/achievementitems/none.png",
":/images/achievementitems/starbronze.png",
":/images/achievementitems/starsilver.png",
":/images/achievementitems/stargold.png"
};
static const int kRotateClassicStage[] = {500, 1000, 2000, -1};
static const char *kRotateClassicTitle[] =
{
"RotateClassic",
"RotateClassic 1",
"RotateClassic 2",
"RotateClassic 3"
};
static const char *kRotateClassicPaths[] =
{
":/images/achievementitems/none.png",
":/images/achievementitems/rotatebronze.png",
":/images/achievementitems/rotatesilver.png",
":/images/achievementitems/rotategold.png"
};
static const int kTimingStage[] =        {400, 700, 1000, -1};
static const char *kTimingTitle[] =
{
"Timing",
"Timing 1",
"Timing 2",
"Timing 3"
};
static const char *kTimingPaths[] =
{
":/images/achievementitems/none.png",
":/images/achievementitems/timingbronze.png",
":/images/achievementitems/timingsilver.png",
":/images/achievementitems/timinggold.png"
};
static const char *kRotatePuzzleTitle[] =
{
"Rotate Puzzle",
"Rotate Puzzle Half",
"Rotate Puzzle Finished"};
static const char *kRotatePuzzlePaths[] =
{
":/images/achievementitems/none.png",
":/images/achievementitems/puzzlehalf.png",
":/images/achievementitems/puzzleall.png"
};


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

  /**
   * @brief Paint the newly got achievements on the screen.
   */
  virtual void paint(QPainter *painter, int width, int height);

  /**
   * @brief Advance the object to realize some animations.
   */
  void advance();

  /**
   * @brief Get the pointer of the item of the achievement.
   */
  static AbstractAchievementItem *getAchievementItem
      (AbstractAchievementItem::ItemType type, QPainter *painter=NULL);

  /**
   * @brief Get the pointers of all the items.
   */
  static QVector<AbstractAchievementItem *> getAchievementItems(QPainter *painter=NULL);

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
