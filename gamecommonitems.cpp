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

#define BUTTON_LOGICAL_WIDTH  120
#define BUTTON_LOGICAL_HEIGHT 60

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
void drawTextAt(double x, double y, QPainter *painter, QString text)
{
  QFontMetrics fm = painter->fontMetrics();
  QPoint pt(x - fm.width(text) / 2,
            y - (fm.ascent() + fm.descent()) / 2 + fm.ascent());
  painter->drawText(pt,text);
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
  int innerBottom = y + 540 * yRate / 2;
  int innerTop = y + (1.0 * foreground.height() / 2 - 10.0 - percentage * 482.0) * yRate;

  // Paint the background of the progress bar
  painter->drawPixmap(x - background.width() * xRate / 2,
                      y - background.height() * yRate / 2,
                      background.width() * xRate,
                      background.height() * yRate,
                      background,
                      0,
                      0,
                      background.width(),
                      background.height());

  // Paint the foreground of the progress bar
  painter->drawPixmap(x - foreground.width() * xRate / 2,
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

FlameItem::FlameItem()
{
  // Set the icon
  p = QPixmap(":/images/bonus/flame.png");

  // Set the 2 values
  setMax(99);
  setCurrent(0);
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
  y += 5;

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
  drawTextAt(x, y, painter, text);

  // Set the font to the original one
  painter->setFont(originalFont);
}

void FlameItem::paintLocatingIcon(QPainter *painter,
                                  QPointF pos,
                                  int frame)
{
  // Paint the icon
  drawPixmapAt(painter,
               p,
               1,
               1,
               pos,
               false,
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
  drawTextAt(x, y, painter, text);

  // Set the font to the original one
  painter->setFont(originalFont);
}

void StarItem::paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame)
{
  // Paint the icon
  drawPixmapAt(painter,
               p,
               1,
               1,
               pos,
               false,
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

void HintItem::paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)
{
  // Init buttons if neccessary
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();

  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[HINT][frame % buttonsFrameCounts[HINT]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();

  // Paint the icon
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

void ExitItem::paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)
{
  // Init buttons if neccessary
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();

  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[EXIT][frame % buttonsFrameCounts[EXIT]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();

  // Paint the icon
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

void ResetItem::paint(QPainter *painter,
                      int width,
                      int height,
                      int frame)
{
  // Init buttons if neccessary
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();

  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[RESET][frame % buttonsFrameCounts[RESET]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();

  // Paint the icon
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

void PauseItem::paint(QPainter *painter,
                      int width,
                      int height,
                      int frame)
{
  // Init buttons if neccessary
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();

  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[PAUSE][frame % buttonsFrameCounts[PAUSE]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();

  // Paint the icon
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

void ConfirmItem::paint(QPainter *painter,
                        int width,
                        int height,
                        int frame)
{
  // Init buttons if neccessary
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();

  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[CONFIRM][frame % buttonsFrameCounts[CONFIRM]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();

  // Paint the icon
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

void CancelItem::paint(QPainter *painter,
                       int width,
                       int height,
                       int frame)
{
  // Init buttons if neccessary
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();

  // Calculate values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[CANCEL][frame % buttonsFrameCounts[CANCEL]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();

  // Paint the icon
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

IntegerItem::IntegerItem()
{
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

  // Calculate values to locate
  QString text1 = QObject::tr("%1").arg(getValue());
  double totalHeight = 70 * height / LOGICAL_HEIGHT;
  double width1 = wordWidth(painter, text1);
  double width2 = wordWidth(painter, hint);
  double totalWidth = qMax(width1 * 2, width2 * 1.33);

  // Draw the background
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

