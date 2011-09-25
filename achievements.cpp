#include "achievements.h"

#include <QPainter>

#define LAST_TIME 100
#define ANIM_TIME 20
#define MAX_X     0.25
#define Y         0.2

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
    double x;
    if (count < ANIM_TIME)
      x = MAX_X * 2 * count / ANIM_TIME - MAX_X;
    else if (count > LAST_TIME - ANIM_TIME)
      x = MAX_X * 2 * (LAST_TIME - count) / ANIM_TIME - MAX_X;
    else
      x = MAX_X;
    items[0]->setPos(QPointF(x, Y));
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
  // TODO
  levelFrom = 0;
  levelTo = 1;
//  levelFrom = calculateLevel(type, lastValue);
//  levelTo = calculateLevel(type, currentValue);
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
      items.push_back(item);
    item = getAchievementItem(itemType);
    if (item != NULL)
      items.push_back(item);
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
  int value;
  AbstractAchievementItem *item = NULL;
  switch (type)
  {
  case AbstractAchievementItem::FlameGet:
  case AbstractAchievementItem::StarGet:
  case AbstractAchievementItem::RotateClassic:
  case AbstractAchievementItem::Timing:
  case AbstractAchievementItem::RotatePuzzle:
    value = statistic.getStatistic(Statistic::FlameGetCount);
    item = new FlameGetItem
           (getAchievementLevel(AbstractAchievementItem::FlameGet), value);
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
