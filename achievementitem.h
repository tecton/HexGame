#ifndef ACHIEVEMENTITEM_H
#define ACHIEVEMENTITEM_H

#include "abstractitem.h"

#include <QPixmap>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QPainterPath>

#define DESCRIPTION_AGE_LIMIT 30

/**
 * @brief An abstract class of items used to show brief infomation about an
 * achievement.
 */
class AbstractAchievementItem : public AbstractCircleItem
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

  virtual void paint(QPainter *painter, int width, int height, int frame);

  /**
   * @brief Paint the description of the achievement in the given rect.
   */
  void paintDescription(QPainter *painter,
                        double width,
                        double height,
                        int frame);

  virtual double r();

  /**
   * @brief Advance the description.
   */
  inline void advanceDescription()
  {
    ++descriptionAge;
  }

  /**
   * @brief Lose the description.
   */
  inline void loseDescriptionFocus()
  {
    rotation += descriptionAge;
    descriptionAge = 0;
  }

  /**
   * @brief Get the age of description.
   */
  inline int getDescriptionAge()
  {return descriptionAge;}

  /**
   * @brief Set the age of description.
   */
  inline void setDescriptionAge(int v)
  {descriptionAge = v;}

  /**
   * @brief Get the title.
   */
  inline QString getTitle()
  {return title;}

  /**
   * @brief Set the title.
   */
  inline void setTitle(QString str)
  {title = str;}

  /**
   * @brief Set the description.
   */
  inline void setDescriptionPath(QPainterPath path)
  {descriptionPath = path;}

  /**
   * @brief Get the description.
   */
  inline QPainterPath getDescriptionPath()
  {return descriptionPath;}

  /**
   * @brief Set the background.
   */
  inline void setBackground(const QPixmap& pixmap)
  {background = pixmap;}

  /**
   * @brief Get the rotation.
   */
  inline int getRotation()
  {return rotation;}

  /**
   * @brief Set the rotation.
   */
  inline void setRotation(double theRotation)
  {rotation = theRotation;}

private:
  int rotation;
  int descriptionAge;
  QString title;
  QPixmap background;
  QPainterPath descriptionPath;
};

/**
 * @brief A class of items used to show number of flames the user has got.
 */
class FlameGetItem : public AbstractAchievementItem
{
public:
  /**
   * @brief Constructor.
   */
  FlameGetItem(int theLevel, int theCurrent, QPainter *painter=NULL);

private:
  int level;
  int current;
  int next;
};

/**
 * @brief A class of items used to show number of stars the user has got.
 */
class StarGetItem : public AbstractAchievementItem
{
public:
  /**
   * @brief Constructor.
   */
  StarGetItem(int theLevel, int theCurrent, QPainter *painter=NULL);

private:
  int level;
  int current;
  int next;
};

/**
 * @brief A class of items used to show the highest score the user has got in
 * rotate classic game.
 */
class RotateClassicPointItem : public AbstractAchievementItem
{
public:
  /**
   * @brief Constructor.
   */
  RotateClassicPointItem(int theLevel, int theCurrent, QPainter *painter=NULL);

private:
  int level;
  int current;
  int next;
};

/**
 * @brief A class of items used to show the highest score the user has got in
 * timing game.
 */
class TimingPointItem : public AbstractAchievementItem
{
public:
  /**
   * @brief Constructor.
   */
  TimingPointItem(int theLevel, int theCurrent, QPainter *painter=NULL);

private:
  int level;
  int current;
  int next;
};

/**
 * @brief A class of items used to show the stages finished in rotate puzzle
 * game.
 */
class RotatePuzzleFinishedItem : public AbstractAchievementItem
{
public:
  /**
   * @brief Constructor.
   */
  RotatePuzzleFinishedItem(int theFinished, int theTotal, QPainter *painter=NULL);

private:
  int finished;
  int total;
};

#endif // ACHIEVEMENTITEM_H
