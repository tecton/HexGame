#include "achievements.h"

#include <QPainter>

#define LAST_TIME 100
#define ANIM_TIME 20

#define MAX_X     0.7578
#define MAX_Y     0.2

extern Statistic statistic;

Achievements::Achievements() :
    count(0)
{
}

void Achievements::paint(QPainter *painter, int width, int height)
{
  if (!items.isEmpty())
    items[0]->paint(painter, width / 2, height / 2, count);
}

void Achievements::advance()
{
  if (count == LAST_TIME && (!items.isEmpty()))
  {
    delete items[0];
    items.pop_front();
    count = 0;
  }
  if (!items.isEmpty())
  {
    double y;
    if (count < ANIM_TIME)
      y = MAX_Y * 2 * count / ANIM_TIME - MAX_Y;
    else if (count > LAST_TIME - ANIM_TIME)
      y = MAX_Y * 2 * (LAST_TIME - count) / ANIM_TIME - MAX_Y;
    else
      y = MAX_Y;
    items[0]->setPos(QPointF(MAX_X, y));
    ++count;
  }
  else
    count = 0;
}

int calculateLevel(AbstractAchievementItem::ItemType type, int value)
{
  int count;
  const int *stages;
  switch (type)
  {
  case AbstractAchievementItem::FlameGet:
    count = 3;
    stages = kFlameGetStage;
    break;
  case AbstractAchievementItem::StarGet:
    count = 3;
    stages = kStarGetStage;
    break;
  case AbstractAchievementItem::RotateClassic:
    count = 3;
    stages = kRotateClassicStage;
    break;
  case AbstractAchievementItem::Timing:
    count = 3;
    stages = kTimingStage;
    break;
  default:
    return 0;
    break;
  }
  for (int i = count - 1;i >= 0;--i)
    if (value >= stages[i])
      return i + 1;
  return 0;
}

void calculateLevel(AbstractAchievementItem::ItemType type,
                    int lastValue,
                    int currentValue,
                    int& levelFrom,
                    int& levelTo)
{
  levelFrom = calculateLevel(type, lastValue);
  levelTo = calculateLevel(type, currentValue);
}

AbstractAchievementItem::ItemType typeConvert(Statistic::StatisticType type)
{
  switch (type)
  {
  case Statistic::FlameGetCount:
    return AbstractAchievementItem::FlameGet;
    break;
  case Statistic::StarGetCount:
    return AbstractAchievementItem::StarGet;
    break;
  case Statistic::RotateClassicPoint:
    return AbstractAchievementItem::RotateClassic;
    break;
  case Statistic::SwapTimingPoint:
  case Statistic::RotateTimingPoint:
    return AbstractAchievementItem::Timing;
    break;
  case Statistic::RotatePuzzleFinished:
    return AbstractAchievementItem::RotatePuzzle;
    break;
  default:
    return (AbstractAchievementItem::ItemType)-1;
    break;
  }
}

void Achievements::statisticChanged(Statistic::StatisticType type,
                                    int lastValue,
                                    int currentValue)
{
  int levelFrom;
  int levelTo;
  AbstractAchievementItem::ItemType itemType = typeConvert(type);
  calculateLevel(itemType, lastValue, currentValue, levelFrom, levelTo);
  if (levelTo > levelFrom)
  {
    AbstractAchievementItem *item = getAchievementItem(itemType);
    if (item != NULL)
    {
      item->setPos(QPointF(MAX_X, -MAX_Y));
      items.push_back(item);
    }
  }
}

int Achievements::getAchievementLevel(AbstractAchievementItem::ItemType type,
                                      int value)
{
  if (value == -1)
  {
    switch (type)
    {
    case AbstractAchievementItem::FlameGet:
      value = statistic.getStatistic(Statistic::FlameGetCount);
      break;
    case AbstractAchievementItem::StarGet:
      value = statistic.getStatistic(Statistic::StarGetCount);
      break;
    case AbstractAchievementItem::RotateClassic:
      value = statistic.getStatistic(Statistic::RotateClassicPoint);
      break;
    case AbstractAchievementItem::Timing:
      value = qMax(statistic.getStatistic(Statistic::SwapTimingPoint),
                   statistic.getStatistic(Statistic::RotateTimingPoint));
      break;
    default:
      return 0;
      break;
    }
  }
  return calculateLevel(type, value);
}

AbstractAchievementItem *Achievements::getAchievementItem
      (AbstractAchievementItem::ItemType type)
{
  AbstractAchievementItem *item = NULL;
  switch (type)
  {
  case AbstractAchievementItem::FlameGet:
    item = new FlameGetItem
           (getAchievementLevel(AbstractAchievementItem::FlameGet),
            statistic.getStatistic(Statistic::FlameGetCount));
    break;
  case AbstractAchievementItem::StarGet:
    item = new StarGetItem
           (getAchievementLevel(AbstractAchievementItem::StarGet),
            statistic.getStatistic(Statistic::StarGetCount));
    break;
  case AbstractAchievementItem::RotateClassic:
    item = new RotateClassicPointItem
           (getAchievementLevel(AbstractAchievementItem::RotateClassic),
            statistic.getStatistic(Statistic::RotateClassicPoint));
    break;
  case AbstractAchievementItem::Timing:
    item = new TimingPointItem
           (getAchievementLevel(AbstractAchievementItem::Timing),
            qMax(statistic.getStatistic(Statistic::SwapTimingPoint),
                 statistic.getStatistic(Statistic::RotateTimingPoint)));
    break;
  case AbstractAchievementItem::RotatePuzzle:
    item = new RotatePuzzleFinishedItem
           (statistic.getStatistic(Statistic::RotatePuzzleFinished),
            statistic.getStatistic(Statistic::RotatePuzzleTotal));
    break;
  default:
    break;
  }
  return item;
}

QVector<AbstractAchievementItem *> Achievements::getAchievementItems()
{
  QVector<AbstractAchievementItem *> result;
  result.push_back(getAchievementItem(AbstractAchievementItem::FlameGet));
  result.push_back(getAchievementItem(AbstractAchievementItem::StarGet));
  result.push_back(getAchievementItem(AbstractAchievementItem::RotateClassic));
  result.push_back(getAchievementItem(AbstractAchievementItem::Timing));
  result.push_back(getAchievementItem(AbstractAchievementItem::RotatePuzzle));
  return result;
}
