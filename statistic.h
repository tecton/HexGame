#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>

class Statistic : public QObject
{
  Q_OBJECT
public:
  enum StatisticType {FlameGetCount=0,
                      StarGetCount,
                      FlameUsedCount,
                      StarUsedCount,
                      GoodMoveCount,
                      BadMoveCount,
                      SwapClassicPoint,
                      RotateClassicPoint,
                      SwapEndlessPoint,
                      RotateEndlessPoint,
                      SwapTimingPoint,
                      RotateTimingPoint,
                      RotatePuzzleFinished,
                      RotatePuzzleTotal};

  int getStatistic(StatisticType type);
  void changeStatistic(StatisticType type, int value, bool add = false);

signals:
  void statisticChanged(Statistic::StatisticType type,
                        int lastValue,
                        int currentValue);
};

#endif // STATISTIC_H
