#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include <QObject>

#include "achievementitem.h"
#include "statistic.h"
#include <QVector>

const int kFlameGetStage[] =      {10, 100, 1000};
const int kStarGetStage[] =       {10, 100, 1000};
const int kRotateClassicStage[] = {500, 1000, 2000};
const int kTimingStage[] =        {400, 700, 1000};

class QPainter;

class Achievements : public QObject
{
  Q_OBJECT
public:
  Achievements();

  virtual void paint(QPainter *painter, int width, int height);
  void advance();


  static AbstractAchievementItem *getAchievementItem
      (AbstractAchievementItem::ItemType type);
  static QVector<AbstractAchievementItem *> getAchievementItems();
  static int getAchievementLevel(AbstractAchievementItem::ItemType type,
                                 int value = -1);

private:
  QVector <AbstractAchievementItem *> items;
  int count;

public slots:
  void statisticChanged(Statistic::StatisticType type,
                        int lastValue,
                        int currentValue);
};

#endif // ACHIEVEMENTS_H
