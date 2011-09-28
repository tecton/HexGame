#include "achievementitem.h"

#include <QPainter>
#include <QColor>
#include "achievements.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH       1024
#define LOGICAL_HEIGHT      600
#define ITEM_WIDTH          400.0
#define ITEM_HEIGHT         80.0
#define DESCRIPTION_WIDTH   500
#define DESCRIPTION_HEIGHT  450

#define FADE_IN_FRAMES  10

double AbstractAchievementItem::width()
{
  return ITEM_WIDTH;
}

double AbstractAchievementItem::height()
{
  return ITEM_HEIGHT;
}

void AbstractAchievementItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  drawPixmapAt(painter,
               background,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);

  painter->setPen(QColor(255,0,0));
  painter->drawText(x, y, title);
}

void AbstractAchievementItem::paintDescription(QPainter *painter,
                                               QRectF rect,
                                               int frame)
{
  if (descriptionAge < FADE_IN_FRAMES)
    painter->setOpacity(1.0 * descriptionAge / FADE_IN_FRAMES);
  painter->setPen(QColor(255,0,0,255*descriptionAge/DESCRIPTION_AGE_LIMIT));
  painter->drawRoundRect(rect, 25 * rect.width() / DESCRIPTION_WIDTH, 25 * rect.height() / DESCRIPTION_HEIGHT);
  painter->drawText(rect, Qt::AlignCenter, description);
  painter->setOpacity(1);
}

FlameGetItem::FlameGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kFlameGetStage[level];
  setTitle(kFlameGetTitle[level]);
  if (next != -1)
    setDescription(QObject::tr("Level %1\nYou've got %2 flames\nGet %3 flames to reach next stage\n").arg(level).arg(current).arg(next-current));
  else
    setDescription(QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level));
}

StarGetItem::StarGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kStarGetStage[level];
  setTitle(kStarGetTitle[level]);
  if (next != -1)
    setDescription(QObject::tr("Level %1\nYou've got %2 stars\nGet %3 stars to reach next stage\n").arg(level).arg(current).arg(next-current));
  else
    setDescription(QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level));
}

RotateClassicPointItem::RotateClassicPointItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kRotateClassicStage[level];
  setTitle(kRotateClassicTitle[level]);
  if (next != -1)
    setDescription(QObject::tr("Level %1\nGet %2 points in rotate classic game to reach next level\n").arg(level).arg(next));
  else
    setDescription(QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level));
}

TimingPointItem::TimingPointItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kTimingStage[level];
  setTitle(kTimingTitle[level]);
  if (next != -1)
    setDescription(QObject::tr("Level %1\nGet %2 points in timing game to reach next level\n").arg(level).arg(next));
  else
    setDescription(QObject::tr("Level %1\nIt's already the MAX LEVEL").arg(level));
}

RotatePuzzleFinishedItem::RotatePuzzleFinishedItem(int theFinished,
                                                   int theTotal) :
        finished(theFinished),
        total(theTotal)
{
  setTitle(kRotatePuzzleTitle[0]);
  if (theFinished != theTotal)
    setDescription(QObject::tr("%1 of %2 puzzle(s) cleared ").arg(finished).arg(total));
  else
    setDescription(QObject::tr("All %1 puzzles cleared ").arg(total));
}
