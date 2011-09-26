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
class AbstractAchievementItem : public AbstractRectItem
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
   * @brief Get the description of the achievement.
   */
  virtual QString getDescription()=0;

  /**
   * @brief Paint the description of the achievement in the given rect.
   */
  void paintDescription(QPainter *painter, QRectF rect, int frame);

  virtual double width();
  virtual double height();
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
  virtual QString getDescription()
  {return description;}

private:
  int level;
  int current;
  int next;
  QString description;
};

/**
 * @brief A class of items used in achievement widget to show number of flames
 * the user has got.
 */
class StarGetItem : public AbstractAchievementItem
{
public:
  StarGetItem(int theLevel, int theCurrent);

  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual QString getDescription()
  {return description;}

private:
  int level;
  int current;
  int next;
  QString description;
};

/**
 * @brief A class of items used in achievement widget to show the highest score
 * the user has got in rotate classic game.
 */
class RotateClassicPointItem : public AbstractAchievementItem
{
public:
  RotateClassicPointItem(int theLevel, int theCurrent);

  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual QString getDescription()
  {return description;}

private:
  int level;
  int current;
  int next;
  QString description;
};

/**
 * @brief A class of items used in achievement widget to show the highest score
 * the user has got in timing game.
 */
class TimingPointItem : public AbstractAchievementItem
{
public:
  TimingPointItem(int theLevel, int theCurrent);

  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual QString getDescription()
  {return description;}

private:
  int level;
  int current;
  int next;
  QString description;
};

/**
 * @brief A class of items used in achievement widget to show the stages
 * finished in rotate puzzle game.
 */
class RotatePuzzleFinishedItem : public AbstractAchievementItem
{
public:
  RotatePuzzleFinishedItem(int theFinished, int theTotal);

  virtual void paint(QPainter *painter, int width, int height, int frame);
  virtual QString getDescription()
  {return description;}

private:
  int finished;
  int total;
  QString description;
};



#endif // ACHIEVEMENTITEM_H
