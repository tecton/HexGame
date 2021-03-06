#include "achievementitem.h"

#include <QPainter>
#include <QColor>
#include <QFontMetrics>
#include "achievements.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH       1024
#define LOGICAL_HEIGHT      600
#define DESCRIPTION_WIDTH   500
#define DESCRIPTION_HEIGHT  450

#define FADE_IN_FRAMES      10

#define ITEM_R              90

#define FONT_DIGIT_SIZE     30

#define LONGEST_STRING      "in rotate classic game"

double AbstractAchievementItem::r()
{
  return ITEM_R;
}

/**
 * @brief Set the font of a description to a painter.
 */
void setDescriptionFont(QPainter *painter)
{
  QFont f;
  f.setBold(true);
  f.setPointSize(FONT_DIGIT_SIZE);
  painter->setFont(f);
  double width = painter->fontMetrics().width(LONGEST_STRING);
  if (width > DESCRIPTION_WIDTH)
    f.setPointSize(FONT_DIGIT_SIZE * DESCRIPTION_WIDTH / width);
  painter->setFont(f);
}

/**
 * @brief Add text to a painter path.
 */
void addText(QPainter *painter,
             QFontMetrics& fontMetrics,
             QPainterPath& path,
             QString str,
             double y)
{
  double x = 250 - fontMetrics.width(str) / 2;
  path.addText(x, y, painter->font(), str);
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
  painter->rotate(-rotation - descriptionAge);

  drawPixmapAt(painter,
               background,
               xRate,
               yRate,
               QPointF(0, 0),
               true,
               true);

  painter->rotate(rotation + descriptionAge);
  painter->translate(-x, -y);
}

void AbstractAchievementItem::paintDescription(QPainter *painter,
                                               double width,
                                               double height,
                                               int frame)
{
  painter->scale(width / DESCRIPTION_WIDTH,
                 height / DESCRIPTION_HEIGHT);
  if (descriptionAge < FADE_IN_FRAMES)
    painter->setOpacity(1.0 * descriptionAge / FADE_IN_FRAMES);
  QPen pen = QPen(QColor(255, 255, 255));
  pen.setWidth(3);
  painter->setPen(pen);
  QRect rect = QRect(0,
                     0,
                     DESCRIPTION_WIDTH,
                     DESCRIPTION_HEIGHT);
  painter->drawRoundRect(rect, 25, 25);
  painter->drawPath(descriptionPath);
  painter->fillPath(descriptionPath, QBrush(QColor(0, 0, 0)));
  painter->setOpacity(1);
  painter->scale(DESCRIPTION_WIDTH / width,
                 DESCRIPTION_HEIGHT / height);
}

FlameGetItem::FlameGetItem(int theLevel, int theCurrent, QPainter *painter) :
    level(theLevel),
    current(theCurrent)
{
  next = kFlameGetStage[level];
  setTitle(kFlameGetTitle[level]);
  setBackground(QPixmap(kFlameGetPaths[theLevel]));
  if (!painter)
    return;
  setDescriptionFont(painter);
  QFontMetrics fontMetrics = painter->fontMetrics();
  QPainterPath path;
  if (next != -1)
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 90);
    addText(painter, fontMetrics, path, QObject::tr("You've got %1 flames").arg(current), 190);
    addText(painter, fontMetrics, path, QObject::tr("Get %1 flames").arg(next-current), 290);
    addText(painter, fontMetrics, path, QObject::tr("to reach next stage"), 390);
  }
  else
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 140);
    addText(painter, fontMetrics, path, QObject::tr("It's already the"), 240);
    addText(painter, fontMetrics, path, QObject::tr("MAX LEVEL"), 340);
  }
  setDescriptionPath(path);
}

StarGetItem::StarGetItem(int theLevel, int theCurrent, QPainter *painter) :
    level(theLevel),
    current(theCurrent)
{
  next = kStarGetStage[level];
  setTitle(kStarGetTitle[level]);
  setBackground(QPixmap(kStarGetPaths[theLevel]));
  setRotation(0);
  if (!painter)
    return;
  setDescriptionFont(painter);
  QFontMetrics fontMetrics = painter->fontMetrics();
  QPainterPath path;
  if (next != -1)
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 90);
    addText(painter, fontMetrics, path, QObject::tr("You've got %1 stars").arg(current), 190);
    addText(painter, fontMetrics, path, QObject::tr("Get %1 stars").arg(next-current), 290);
    addText(painter, fontMetrics, path, QObject::tr("to reach next stage"), 390);
  }
  else
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 140);
    addText(painter, fontMetrics, path, QObject::tr("It's already the"), 240);
    addText(painter, fontMetrics, path, QObject::tr("MAX LEVEL"), 340);
  }
  setDescriptionPath(path);
}

RotateClassicPointItem::RotateClassicPointItem(int theLevel, int theCurrent, QPainter *painter) :
    level(theLevel),
    current(theCurrent)
{
  next = kRotateClassicStage[level];
  setTitle(kRotateClassicTitle[level]);
  setBackground(QPixmap(kRotateClassicPaths[theLevel]));
  setRotation(0);
  if (!painter)
    return;
  setDescriptionFont(painter);
  QFontMetrics fontMetrics = painter->fontMetrics();
  QPainterPath path;
  if (next != -1)
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 90);
    addText(painter, fontMetrics, path, QObject::tr("Get %1 points").arg(next), 190);
    addText(painter, fontMetrics, path, QObject::tr("in rotate classic game"), 290);
    addText(painter, fontMetrics, path, QObject::tr("to reach next level"), 390);
  }
  else
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 140);
    addText(painter, fontMetrics, path, QObject::tr("It's already the"), 240);
    addText(painter, fontMetrics, path, QObject::tr("MAX LEVEL"), 340);
  }
  setDescriptionPath(path);
}

TimingPointItem::TimingPointItem(int theLevel, int theCurrent, QPainter *painter) :
    level(theLevel),
    current(theCurrent)
{
  next = kTimingStage[level];
  setTitle(kTimingTitle[level]);
  setBackground(QPixmap(kTimingPaths[theLevel]));
  setRotation(0);
  if (!painter)
    return;
  setDescriptionFont(painter);
  QFontMetrics fontMetrics = painter->fontMetrics();
  QPainterPath path;
  if (next != -1)
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 90);
    addText(painter, fontMetrics, path, QObject::tr("Get %1 points").arg(next), 190);
    addText(painter, fontMetrics, path, QObject::tr("in timing game"), 290);
    addText(painter, fontMetrics, path, QObject::tr("to reach next level"), 390);
  }
  else
  {
    addText(painter, fontMetrics, path, QObject::tr("Level %1").arg(level), 140);
    addText(painter, fontMetrics, path, QObject::tr("It's already the"), 240);
    addText(painter, fontMetrics, path, QObject::tr("MAX LEVEL"), 340);
  }
  setDescriptionPath(path);
}

RotatePuzzleFinishedItem::RotatePuzzleFinishedItem(int theFinished,
                                                   int theTotal,
                                                   QPainter *painter) :
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
  if (!painter)
    return;
  setDescriptionFont(painter);
  QFontMetrics fontMetrics = painter->fontMetrics();
  QPainterPath path;
  switch (index)
  {
  case 0:
    addText(painter, fontMetrics, path, QObject::tr("New to"), 190);
    addText(painter, fontMetrics, path, QObject::tr("rotate puzzle game"), 290);
    break;
  case 1:
    addText(painter, fontMetrics, path, QObject::tr("%1 of %2").arg(finished).arg(total), 190);
    addText(painter, fontMetrics, path, QObject::tr("puzzle(s) cleared"), 290);
    break;
  case 2:
    addText(painter, fontMetrics, path, QObject::tr("All %1").arg(total), 190);
    addText(painter, fontMetrics, path, QObject::tr("puzzles cleared"), 290);
    break;
  }
  setDescriptionPath(path);
}
