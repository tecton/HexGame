#ifndef ACHIEVEMENTITEM_H
#define ACHIEVEMENTITEM_H

#include "abstractitem.h"

#include <QPixmap>
#include <QPointF>
#include <QString>
#include <QVector>

#define DESCRIPTION_AGE_LIMIT 30

/**
 * @brief An abstract class of items used to show brief infomation about an
 * achievement.
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

  inline void advanceDescription()
  {
    if (descriptionAge < DESCRIPTION_AGE_LIMIT)
      ++descriptionAge;
  }

  inline void loseDescriptionFocus()
  {descriptionAge = 0;}

  inline int getDescriptionAge()
  {return descriptionAge;}

  inline void setDescriptionAge(int v)
  {descriptionAge = v;}

private:
  int descriptionAge;
};

/**
 * @brief A class of items used to show number of flames the user has got.
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
 * @brief A class of items used to show number of stars the user has got.
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
 * @brief A class of items used to show the highest score the user has got in
 * rotate classic game.
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
 * @brief A class of items used to show the highest score the user has got in
 * timing game.
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
 * @brief A class of items used to show the stages finished in rotate puzzle
 * game.
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
