#ifndef STATISTIC_H
#define STATISTIC_H

#include <QObject>

/**
 * @brief A class to deal with the statistics of the game.
 */
class Statistic : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief Type of the statistics.
   *
   * @warning Other infomation isn't stored currently, if you wish to add one
   * or more things, you need to delete the file maintained by the program, and
   * let the program create the file. Otherwise, the size of the file may be
   * and the program may not run correctly.
   */
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

  /**
   * @brief Get a statistic.
   */
  int getStatistic(StatisticType type);

  /**
   * @brief Change a statistic.
   */
  void changeStatistic(StatisticType type, int value, bool add = false);

signals:
  /**
   * @brief A signal to tell other part of the game that the statistic was
   * changed
   */
  void statisticChanged(Statistic::StatisticType type,
                        int lastValue,
                        int currentValue);
};

#endif // STATISTIC_H
