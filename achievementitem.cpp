#include "achievementitem.h"

#include <QPainter>
#include <QColor>
#include "achievements.h"

#define LOGICAL_WIDTH       1024
#define LOGICAL_HEIGHT      600
#define ITEM_WIDTH          400.0
#define ITEM_HEIGHT         80.0
#define DESCRIPTION_WIDTH   500
#define DESCRIPTION_HEIGHT  450

double AbstractAchievementItem::width()
{
  return ITEM_WIDTH;
}

double AbstractAchievementItem::height()
{
  return ITEM_HEIGHT;
}

void AbstractAchievementItem::paintDescription(QPainter *painter,
                                               QRectF rect,
                                               int frame)
{
  painter->setPen(QColor(255,0,0,255*descriptionAge/DESCRIPTION_AGE_LIMIT));
  painter->drawRoundRect(rect, 25 * rect.width() / DESCRIPTION_WIDTH, 25 * rect.height() / DESCRIPTION_HEIGHT);
  painter->drawText(rect, Qt::AlignCenter, getDescription());
}

FlameGetItem::FlameGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kFlameGetStage[level];
  if (next != -1)
    description = QObject::tr("Level %1\nYou've got %2 flames\nGet %3 flames to reach next stage\n").arg(level).arg(current).arg(next-current);
  else
    description = QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level);
}

void FlameGetItem::paint(QPainter *painter,
                         int width,
                         int height,
                         int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, "Flame");

}

StarGetItem::StarGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kStarGetStage[level];
  if (next != -1)
    description = QObject::tr("Level %1\nYou've got %2 stars\nGet %3 stars to reach next stage\n").arg(level).arg(current).arg(next-current);
  else
    description = QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level);
}

void StarGetItem::paint(QPainter *painter,
                        int width,
                        int height,
                        int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, "Star");

}

RotateClassicPointItem::RotateClassicPointItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kRotateClassicStage[level];
  if (next != -1)
    description = QObject::tr("Level %1\nGet %2 points in rotate classic game to reach next level\n").arg(level).arg(next);
  else
    description = QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level);
}

void RotateClassicPointItem::paint(QPainter *painter,
                                   int width,
                                   int height,
                                   int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, "Rotate classic game");
}

TimingPointItem::TimingPointItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kTimingStage[level];
  if (next != -1)
    description = QObject::tr("Level %1\nGet %2 points in timing game to reach next level\n").arg(level).arg(next);
  else
    description = QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level);
}

void TimingPointItem::paint(QPainter *painter,
                            int width,
                            int height,
                            int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, "Timing game");
}

RotatePuzzleFinishedItem::RotatePuzzleFinishedItem(int theFinished,
                                                   int theTotal) :
        finished(theFinished),
        total(theTotal)
{
  if (theFinished != theTotal)
    description = QObject::tr("%1 of %2 puzzle(s) cleared ").arg(finished).arg(total);
  else
    description = QObject::tr("All %1 puzzles cleared ").arg(total);
}

void RotatePuzzleFinishedItem::paint(QPainter *painter,
                                     int width,
                                     int height,
                                     int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, "Rotate puzzle game");
}
