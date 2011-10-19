#include "gamecommonitems.h"

#include <QPainter>
#include <QVector>
#include <QPixmap>
#include <QFontMetrics>
#include "abstractgameboardinfo.h"
#include "effectpainter.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

#define BUTTON_LOGICAL_WIDTH  160
#define BUTTON_LOGICAL_HEIGHT 60

#define BUTTON_LOGICAL_WIDTH2  33
#define BUTTON_LOGICAL_HEIGHT2 105

#define HINT 0
#define EXIT 1
#define RESET 2
#define PAUSE 3
#define CONFIRM 4
#define CANCEL 5

// Total color number of the buttons
const static int kButtonsCount = 6;

// File path of the buttons
const static char * kButtonsPaths[] = {":/images/buttons/hint*.png",
                                       ":/images/buttons/exit*.png",
                                       ":/images/buttons/reset*.png",
                                       ":/images/buttons/pause*.png",
                                       ":/images/buttons/confirm*.png",
                                       ":/images/buttons/cancel*.png"};

// Pixmaps of the buttons
QVector<QVector<QPixmap> > buttonsPixmaps;

// Total frame count of the buttons
QVector<int> buttonsFrameCounts;

// Get the width of the word
double wordWidth(QPainter *painter, QString text)
{
  QFontMetrics fm = painter->fontMetrics();
  return fm.width(text);
}

// Draw the text at the given place
// The point (x,y) is the center of the text
void drawTextAt(double x, double y, QPainter *painter, QString text, int rotation = 0)
{
  painter->translate(x, y);
  painter->rotate(rotation);
  QFontMetrics fm = painter->fontMetrics();
  QPoint pt(-fm.width(text) / 2,
            -(fm.ascent() + fm.descent()) / 2 + fm.ascent());
  painter->drawText(pt,text);
  painter->rotate(-rotation);
  painter->translate(-x, -y);
}

void initButtonsPixmaps()
{
  initPixmaps(kButtonsCount,
              kButtonsPaths,
              buttonsPixmaps,
              buttonsFrameCounts);
}

VerticalProgressBarItem::VerticalProgressBarItem()
{
  // Set the foreground and the background
  background = QPixmap(":/images/gamecommonitems/bar2.png");
  foreground = QPixmap(":/images/gamecommonitems/bar1.png");

  // Set the 3 values
  setMin(0);
  setMax(30);
  setCurrent(0);
}

void VerticalProgressBarItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double percentage = qMax(0.0, qMin(1.0, 1.0 * (getCurrent() - getMin()) / (getMax() - getMin())));
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  int innerBottom = y + (1.0 * foreground.height() / 2 - 10.0) * yRate;
  int innerTop = y + (1.0 * foreground.height() / 2 - 10.0 - percentage * 482.0) * yRate;

  // Paint the background of the progress bar
  painter->drawPixmap(x - background.width() * xRate / 2 + 5 * xRate,
                      y - background.height() * yRate / 2,
                      background.width() * xRate,
                      background.height() * yRate,
                      background,
                      0,
                      0,
                      background.width(),
                      background.height());

  // Paint the foreground of the progress bar
  painter->drawPixmap(x - foreground.width() * xRate / 2 + 5 * xRate,
                      innerTop,
                      foreground.width() * xRate,
                      innerBottom - innerTop,
                      foreground,
                      0,
                      58,
                      foreground.width(),
                      foreground.height() - 68.0);

  // Save the font
  QFont originalFont = painter->font();

  // Set the font and pen
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * height / LOGICAL_HEIGHT);
  painter->setFont(f);
  painter->setPen(QPen(QColor(0, 0, 255, 255)));

  // Draw the text
  drawTextAt(x,
             y - foreground.height() * yRate / 2 + 30 * height / LOGICAL_HEIGHT,
             painter,
             QObject::tr("%1").arg(getCurrent()));

  // Set the font to the original one
  painter->setFont(originalFont);
}

VerticalProgressBarItem2::VerticalProgressBarItem2()
{
  // Set the 3 values
  setMin(0);
  setMax(30);
  setCurrent(0);
}

void VerticalProgressBarItem2::paint(QPainter *painter,
                                     int width,
                                     int height,
                                     int frame)
{
  double x = getPos().x() * width;
  double y = getPos().y() * height;

  double percentage = qMax(0.0, qMin(1.0, 1.0 * (getCurrent() - getMin()) / (getMax() - getMin())));
  int barFrom = (1 - percentage) * height;
  int barTo = height;

  int xFrom = x - 19 * width / LOGICAL_WIDTH;
  painter->setPen(QColor( 255 * (1 - percentage), 255 * percentage,  50));
  painter->setBrush(QColor( 255 * (1 - percentage), 255 * percentage,  50));
  painter->drawRect(xFrom, barFrom, 37 * width / LOGICAL_WIDTH, barTo - barFrom);
}

double AbstractBonusItem::r()
{
  return 35;
}

FlameItem::FlameItem()
{
  // Set the icon
  p = QPixmap(":/images/bonus/flame.png");

  // Set the 2 values
  setMax(99);
  setCurrent(0);
  setRotation(0);
}

void FlameItem::setRotation(int r)
{
  AbstractBonusItem::setRotation(r);
  if (r > 0)
    p = QPixmap(":/images/bonus/flame_left.png");
  else if (r < 0)
    p = QPixmap(":/images/bonus/flame_right.png");
  else
    p = QPixmap(":/images/bonus/flame.png");
}

void FlameItem::paint(QPainter *painter,
                      int width,
                      int height,
                      int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;

  // Paint the icon
  drawPixmapAt(painter,
               p,
               1.0 * width / LOGICAL_WIDTH,
               1.0 * height / LOGICAL_HEIGHT,
               QPointF(x, y),
               true,
               true);

  // Calculate values to locate
  if (getRotation() == 0)
    y += 5;
  else if (getRotation() > 0)
    x -= 5;
  else
    x += 5;

  // Save the font
  QFont originalFont = painter->font();

  // Set the font and pen
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(0, 0, 255, 255)));

  // Draw the count
  QString text = QObject::tr("%1").arg(getCurrent());
  drawTextAt(x, y, painter, text, getRotation());

  // Set the font to the original one
  painter->setFont(originalFont);
}

void FlameItem::paintLocatingIcon(QPainter *painter,
                                  int width,
                                  int height,
                                  QPointF pos,
                                  int frame)
{
  // Paint the icon
  drawPixmapAt(painter,
               p,
               1.0 * width / LOGICAL_WIDTH,
               1.0 * height / LOGICAL_HEIGHT,
               pos,
               true,
               true);
}

void FlameItem::paintInfluencedArea(int index,
                                    AbstractGameBoardInfo *gameBoard,
                                    EffectPainter *effectPainter,
                                    int frame)
{
  if (index == -1)
    return;

  // Tell the effect painter to add the effect at some position
  effectPainter->bonusEliminationHintAt(index);
  QVector<int> chainAroundIndex = gameBoard->chainAroundIndex(index);
  for (int i = 0;i < chainAroundIndex.size();++i)
    effectPainter->bonusEliminationHintAt(chainAroundIndex[i]);
}

StarItem::StarItem()
{
  // Set the icon
  p = QPixmap(":/images/bonus/star.png");

  // Set the 2 values
  setMax(99);
  setCurrent(0);
  setRotation(0);
}

void StarItem::setRotation(int r)
{
  AbstractBonusItem::setRotation(r);
  if (r > 0)
    p = QPixmap(":/images/bonus/star_left.png");
  else if (r < 0)
    p = QPixmap(":/images/bonus/star_right.png");
  else
    p = QPixmap(":/images/bonus/star.png");
}

void StarItem::paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;

  // Paint the icon
  drawPixmapAt(painter,
               p,
               1.0 * width / LOGICAL_WIDTH,
               1.0 * height / LOGICAL_HEIGHT,
               QPointF(x, y),
               true,
               true);

  // Save the font
  QFont originalFont = painter->font();

  // Set the font and pen
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(255, 255, 0, 255)));

  // Draw the count
  QString text = QObject::tr("%1").arg(getCurrent());
  drawTextAt(x, y, painter, text, getRotation());

  // Set the font to the original one
  painter->setFont(originalFont);
}

void StarItem::paintLocatingIcon(QPainter *painter,
                                 int width,
                                 int height,
                                 QPointF pos,
                                 int frame)
{
  // Paint the icon
  drawPixmapAt(painter,
               p,
               1.0 * width / LOGICAL_WIDTH,
               1.0 * height / LOGICAL_HEIGHT,
               pos,
               true,
               true);
}

void StarItem::paintInfluencedArea(int index,
                                   AbstractGameBoardInfo *gameBoard,
                                   EffectPainter *effectPainter,
                                   int frame)
{
  if (index == -1)
    return;

  // Tell the effect painter to add the effect at some position
  effectPainter->bonusEliminationHintAt(index);
  for (int i = 0;i < 6;++i)
  {
    int current = gameBoard->nearbyIndex(index, i);
    while (current != -1)
    {
      effectPainter->bonusEliminationHintAt(current);
      current = gameBoard->nearbyIndex(current, i);
    }
  }
}

ButtonItem::ButtonItem(QString str)
{
  caption = str;
  background = QPixmap(":/images/buttons/buttonbackground.png");
  rotation = 0;
}

void ButtonItem::setRotation(int r)
{
  rotation = r;
  if (r > 0)
    background = QPixmap(":/images/buttons/buttonbackground_left.png");
  else if (r < 0)
    background = QPixmap(":/images/buttons/buttonbackground_right.png");
  else
    background = QPixmap(":/images/buttons/buttonbackground.png");
}

void ButtonItem::paint(QPainter *painter, int width, int height, int frame)
{
    // Calculate values to locate
    double x = getPos().x() * width;
    double y = getPos().y() * height;
    double xRate = 1.0 * width * this->width() /
                   LOGICAL_WIDTH / background.width();
    double yRate = 1.0 * height * this->height() /
                   LOGICAL_HEIGHT / background.height();

    // Paint the icon
    drawPixmapAt(painter,
                 background,
                 xRate,
                 yRate,
                 QPointF(x, y),
                 true,
                 true);

    // Save the font
    QFont originalFont = painter->font();

    // Set the font and pen
    QFont f;
    f.setBold(true);
    f.setPixelSize(20 * width / LOGICAL_WIDTH);
    painter->setFont(f);
    painter->setPen(QColor(255, 255, 255));
    // Paint the text
    drawTextAt(x, y, painter, caption, getRotation());

    // Set the font to the original one
    painter->setFont(originalFont);
}


double ButtonItem::width()
{
  if (rotation == 0)
    return BUTTON_LOGICAL_WIDTH;
  return BUTTON_LOGICAL_WIDTH2;
}

double ButtonItem::height()
{
  if (rotation == 0)
    return BUTTON_LOGICAL_HEIGHT;
  return BUTTON_LOGICAL_HEIGHT2;
}

IntegerItem::IntegerItem()
{
  p = QPixmap(":/images/gamecommonitems/label.png");

  rotation = 0;
}

void IntegerItem::setRotation(int r)
{
  rotation = r;
  if (r > 0)
    p = QPixmap(":/images/gamecommonitems/label_right.png");
  else if (r < 0)
    p = QPixmap(":/images/gamecommonitems/label_left.png");
  else
    p = QPixmap(":/images/gamecommonitems/label.png");
}

void IntegerItem::paint(QPainter *painter,
                        int width,
                        int height,
                        int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;

  // Save the font
  QFont originalFont = painter->font();

  // Set the font and pen
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(255, 120, 0, 255)));

  QString text1 = QObject::tr("%1").arg(getValue());

  // Draw the background
  if (rotation == 0)
  {
    // Calculate values to locate
    double totalHeight = 70 * height / LOGICAL_HEIGHT;
    double width1 = wordWidth(painter, text1);
    double width2 = wordWidth(painter, hint);
    double totalWidth = qMax(width1 * 2, width2 * 1.33);
    drawPixmapAt(painter,
                 p,
                 totalWidth / p.width(),
                 totalHeight / p.height(),
                 QPointF(x, y),
                 true,
                 true);

    // Draw the text
    drawTextAt(x, y - 10 * width / LOGICAL_WIDTH, painter, text1);

    // Set the font and pen
    f.setPixelSize(15 * width / LOGICAL_WIDTH);
    painter->setFont(f);
    painter->setPen(QPen(QColor(0, 0, 255, 255)));

    // Draw the text
    drawTextAt(x, y + 15 * width / LOGICAL_WIDTH, painter, hint);
  }
  else
  {
    drawPixmapAt(painter,
                 p,
                 1.0 * width / LOGICAL_WIDTH,
                 1.0 * height / LOGICAL_HEIGHT,
                 QPointF(x, y),
                 true,
                 true);

    // Draw the text
    drawTextAt(x, y, painter, text1, rotation);
  }

  // Set the font to the original one
  painter->setFont(originalFont);
}

StringItem::StringItem()
{
  p = QPixmap(":/images/gamecommonitems/label.png");
}

void StringItem::paint(QPainter *painter,
                       int width,
                       int height,
                       int frame)
{
  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;

  // Save the font
  QFont originalFont = painter->font();

  // Set the font and pen
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(255, 120, 0, 255)));

  // Calculate values to locate
  double totalHeight = 70 * height / LOGICAL_HEIGHT;
  double totalWidth = wordWidth(painter, hint) * 2;

  // Draw the background
  drawPixmapAt(painter,
               p,
               totalWidth / p.width(),
               totalHeight / p.height(),
               QPointF(x, y),
               true,
               true);

  // Draw the text
  drawTextAt(x, y - 10 * width / LOGICAL_WIDTH, painter, hint);

  // Set the font to the original one
  painter->setFont(originalFont);
}

