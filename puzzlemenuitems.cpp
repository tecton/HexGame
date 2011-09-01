#include "puzzlemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

const static int kTotalItems = 3;
const static char * kItemPaths[] = {":/images/puzzlemenuitems/exchange_theme*.png",
                                    ":/images/puzzlemenuitems/unite_theme*.png",
                                    ":/images/puzzlemenuitems/lock_theme*.png"};


QVector<QVector<QPixmap> > puzzleMenuItemPixmaps;
QVector<int> puzzleMenuItemFrameCounts;
void initPuzzleMenuItemPixmaps()
{
  initPixmaps(kTotalItems,
              kItemPaths,
              puzzleMenuItemPixmaps,
              puzzleMenuItemFrameCounts);
}

const QPixmap& AbstractPuzzleMenuItem::pixmap(ItemType type,
                                              int frame)
{
  if (puzzleMenuItemPixmaps.isEmpty())
    initPuzzleMenuItemPixmaps();
  return puzzleMenuItemPixmaps[type][frame % puzzleMenuItemFrameCounts[type]];
}

PuzzleMenuExchangeItem::PuzzleMenuExchangeItem()
{
  setPos(QPointF(0, 0));
}

void PuzzleMenuExchangeItem::paint(QPainter *painter,
                                   int width,
                                   int height,
                                   int frame)
{
  const QPixmap& pixmap = AbstractPuzzleMenuItem::pixmap(
                            AbstractPuzzleMenuItem::ExchangeItem,
                            frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               pixmap,
               0.75,
               0.75,
               QPointF(x, y),
               true,
               true);
}

PuzzleMenuUniteItem::PuzzleMenuUniteItem()
{
  setPos(QPointF(0, 0));
}

void PuzzleMenuUniteItem::paint(QPainter *painter,
                                int width,
                                int height,
                                int frame)
{
  const QPixmap& pixmap = AbstractPuzzleMenuItem::pixmap(
                            AbstractPuzzleMenuItem::UniteItem,
                            frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               pixmap,
               0.75,
               0.75,
               QPointF(x, y),
               true,
               true);
}

PuzzleMenuLockItem::PuzzleMenuLockItem()
{
  setPos(QPointF(0, 0));
}

void PuzzleMenuLockItem::paint(QPainter *painter,
                               int width,
                               int height,
                               int frame)
{
  const QPixmap& pixmap = AbstractPuzzleMenuItem::pixmap(
                            AbstractPuzzleMenuItem::LockItem,
                            frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               pixmap,
               0.75,
               0.75,
               QPointF(x, y),
               true,
               true);
}
