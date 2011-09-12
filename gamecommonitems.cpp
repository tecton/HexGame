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

const static int kButtonsCount = 6;
const static char * kButtonsPaths[] = {":/images/buttons/hint*.png",
                                       ":/images/buttons/exit*.png",
                                       ":/images/buttons/reset*.png",
                                       ":/images/buttons/pause*.png",
                                       ":/images/buttons/confirm*.png",
                                       ":/images/buttons/cancel*.png"};


QVector<QVector<QPixmap> > buttonsPixmaps;
QVector<int> buttonsFrameCounts;

double wordWidth(QPainter *painter, QString text)
{
  QFontMetrics fm = painter->fontMetrics();
  return fm.width(text);
}

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
  background = QPixmap(":/images/gamecommonitems/bar2.png");
  foreground = QPixmap(":/images/gamecommonitems/bar1.png");
  setMin(0);
  setMax(30);
  setCurrent(0);
}

void VerticalProgressBarItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double percentage = qMax(0.0, qMin(1.0, 1.0 * (getCurrent() - getMin()) / (getMax() - getMin())));
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;
  painter->drawPixmap(x - background.width() * xRate / 2,
                      y - background.height() * yRate / 2,
                      background.width() * xRate,
                      background.height() * yRate,
                      background,
                      0,
                      0,
                      background.width(),
                      background.height());

  painter->drawPixmap(x - foreground.width() * xRate / 2,
                      y + (1.0 * foreground.height() / 2 - 10.0 - percentage * 482.0) * yRate,
                      foreground.width() * xRate,
                      percentage * 482.0 * yRate,
                      foreground,
                      0,
                      (1 - percentage) * 482.0 + 58.0,
                      foreground.width(),
                      percentage * (foreground.height() - 68.0));

  QFont originalFont = painter->font();
  QPen originalPen = painter->pen();
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * height / LOGICAL_HEIGHT);
  painter->setFont(f);
  painter->setPen(QPen(QColor(0, 0, 255, 255)));
  drawTextAt(x,
             y - foreground.height() * yRate / 2 + 30 * height / LOGICAL_HEIGHT,
             painter,
             QObject::tr("%1").arg(getCurrent()));
  painter->setFont(originalFont);
  painter->setPen(originalPen);
}

FlameItem::FlameItem()
{
  p = QPixmap(":/images/bonus/flame.png");
  setMax(99);
  setCurrent(0);
}


void FlameItem::paint(QPainter *painter,
                      int width,
                      int height,
                      int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               p,
               1.0 * width / LOGICAL_WIDTH,
               1.0 * height / LOGICAL_HEIGHT,
               QPointF(x, y),
               true,
               true);
  y += 5;
  QFont originalFont = painter->font();
  QPen originalPen = painter->pen();
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(0, 0, 255, 255)));
  QString text = QObject::tr("%1").arg(getCurrent());
  drawTextAt(x, y, painter, text);
  painter->setFont(originalFont);
  painter->setPen(originalPen);
}

void FlameItem::paintLocatingIcon(QPainter *painter,
                                  QPointF pos,
                                  int frame)
{
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
  effectPainter->bonusEliminationHintAt(index);
  QVector<int> chainAroundIndex = gameBoard->chainAroundIndex(index);
  for (int i = 0;i < chainAroundIndex.size();++i)
    effectPainter->bonusEliminationHintAt(chainAroundIndex[i]);
}

StarItem::StarItem()
{
  p = QPixmap(":/images/bonus/star.png");
  setMax(99);
  setCurrent(0);
}


void StarItem::paint(QPainter *painter,
                     int width,
                     int height,
                     int frame)
{
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               p,
               1.0 * width / LOGICAL_WIDTH,
               1.0 * height / LOGICAL_HEIGHT,
               QPointF(x, y),
               true,
               true);
  QFont originalFont = painter->font();
  QPen originalPen = painter->pen();
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(255, 255, 0, 255)));
  QString text = QObject::tr("%1").arg(getCurrent());
  drawTextAt(x, y, painter, text);
  painter->setFont(originalFont);
  painter->setPen(originalPen);
}

void StarItem::paintLocatingIcon(QPainter *painter,
                                 QPointF pos,
                                 int frame)
{
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
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[HINT][frame % buttonsFrameCounts[HINT]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();
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
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[EXIT][frame % buttonsFrameCounts[EXIT]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();
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
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[RESET][frame % buttonsFrameCounts[RESET]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();
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
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[PAUSE][frame % buttonsFrameCounts[PAUSE]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();
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
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  QFont originalFont = painter->font();
  QPen originalPen = painter->pen();
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(255, 120, 0, 255)));
  QString text1 = QObject::tr("%1").arg(getValue());
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
  drawTextAt(x, y - 10 * width / LOGICAL_WIDTH, painter, text1);
  f.setPixelSize(15 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(0, 0, 255, 255)));
  drawTextAt(x, y + 15 * width / LOGICAL_WIDTH, painter, hint);
  painter->setFont(originalFont);
  painter->setPen(originalPen);
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
  painter->setPen(QColor(255,255,255,255));
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  QFont originalFont = painter->font();
  QPen originalPen = painter->pen();
  QFont f;
  f.setBold(true);
  f.setPixelSize(20 * width / LOGICAL_WIDTH);
  painter->setFont(f);
  painter->setPen(QPen(QColor(255, 120, 0, 255)));
  double totalHeight = 70 * height / LOGICAL_HEIGHT;
  double totalWidth = wordWidth(painter, hint) * 2;
  drawPixmapAt(painter,
               p,
               totalWidth / p.width(),
               totalHeight / p.height(),
               QPointF(x, y),
               true,
               true);
  drawTextAt(x, y - 10 * width / LOGICAL_WIDTH, painter, hint);
  painter->setFont(originalFont);
  painter->setPen(originalPen);
}

void ConfirmItem::paint(QPainter *painter,
                        int width,
                        int height,
                        int frame)
{
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[CONFIRM][frame % buttonsFrameCounts[CONFIRM]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();
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
  if (buttonsPixmaps.isEmpty())
    initButtonsPixmaps();
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  const QPixmap& p = buttonsPixmaps[CANCEL][frame % buttonsFrameCounts[CANCEL]];
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH / p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT / p.height();
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

