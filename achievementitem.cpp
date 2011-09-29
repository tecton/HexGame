#include "achievementitem.h"

#include <QPainter>
#include <QColor>
#include "achievements.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH       1024
#define LOGICAL_HEIGHT      600
#define DESCRIPTION_WIDTH   500
#define DESCRIPTION_HEIGHT  450

#define FADE_IN_FRAMES      10

#define ITEM_R              90

#define FONT_FAMILY         ""
#define FONT_DIGIT_SIZE     30

double AbstractAchievementItem::r()
{
  return ITEM_R;
}

void AbstractAchievementItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * ITEM_R * 2 / background.width() * width / LOGICAL_WIDTH;
  double yRate = 1.0 * ITEM_R * 2 / background.height() * height / LOGICAL_HEIGHT;

  painter->translate(x, y);
  painter->rotate(rotation + descriptionAge);

  drawPixmapAt(painter,
               background,
               xRate,
               yRate,
               QPointF(0, 0),
               true,
               true);

  painter->rotate(-rotation - descriptionAge);
  painter->translate(-x, -y);
}

void AbstractAchievementItem::paintDescription(QPainter *painter,
                                               QRectF rect,
                                               int frame)
{
  double fontScale = qMin(1.0 * rect.width() / DESCRIPTION_WIDTH,
                          1.0 * rect.height() / DESCRIPTION_HEIGHT);
  if (descriptionAge < FADE_IN_FRAMES)
    painter->setOpacity(1.0 * descriptionAge / FADE_IN_FRAMES);
  QFont f;
  f.setFamily(FONT_FAMILY);
  f.setBold(true);
  f.setPointSize(FONT_DIGIT_SIZE * fontScale);
  painter->setFont(f);;
  painter->setPen(QColor(255, 255, 255));
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
  setBackground(QPixmap(kFlameGetPaths[theLevel]));
  if (next != -1)
    setDescription(QObject::tr("Level %1\n\nYou've got %2 flames\n\nGet %3 flames\nto reach next stage\n").arg(level).arg(current).arg(next-current));
  else
    setDescription(QObject::tr("Level %1\n\nIt's already the\nMAX LEVEL").arg(level));
}

StarGetItem::StarGetItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kStarGetStage[level];
  setTitle(kStarGetTitle[level]);
  setBackground(QPixmap(kStarGetPaths[theLevel]));
  setRotation(0);
  if (next != -1)
    setDescription(QObject::tr("Level %1\n\nYou've got %2 stars\n\nGet %3 stars\nto reach next stage\n").arg(level).arg(current).arg(next-current));
  else
    setDescription(QObject::tr("Level %1\n\nIt's already the\nMAX LEVEL").arg(level));
}

RotateClassicPointItem::RotateClassicPointItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kRotateClassicStage[level];
  setTitle(kRotateClassicTitle[level]);
  setBackground(QPixmap(kRotateClassicPaths[theLevel]));
  setRotation(0);
  if (next != -1)
    setDescription(QObject::tr("Level %1\n\nGet %2 points\nin rotate classic game\nto reach next level\n").arg(level).arg(next));
  else
    setDescription(QObject::tr("Level %1\n\nIt's already the\nMAX LEVEL").arg(level));
}

TimingPointItem::TimingPointItem(int theLevel, int theCurrent) :
    level(theLevel),
    current(theCurrent)
{
  next = kTimingStage[level];
  setTitle(kTimingTitle[level]);
  setBackground(QPixmap(kTimingPaths[theLevel]));
  setRotation(0);
  if (next != -1)
    setDescription(QObject::tr("Level %1\n\nGet %2 points\nin timing game\nto reach next level\n").arg(level).arg(next));
  else
    setDescription(QObject::tr("Level %1\n\nIt's already the\nMAX LEVEL").arg(level));
}

RotatePuzzleFinishedItem::RotatePuzzleFinishedItem(int theFinished,
                                                   int theTotal) :
        finished(theFinished),
        total(theTotal)
{
  setTitle(kRotatePuzzleTitle[0]);
  setRotation(0);
  int index;
  if (theFinished == 0)
    index = 0;
  else if (theFinished == theTotal)
    index = 2;
  else
    index = 1;

  setBackground(QPixmap(kRotatePuzzlePaths[index]));
  if (theFinished != theTotal)
    setDescription(QObject::tr("%1 of %2\npuzzle(s) cleared ").arg(finished).arg(total));
  else
    setDescription(QObject::tr("All %1\npuzzles cleared ").arg(total));
}
