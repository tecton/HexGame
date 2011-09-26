#include "puzzlemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

// Total color number of the items
const static int kTotalItems = 3;

// File path of the items
// (The last two are the same as two before them, may make
//  cause some bug later)
const static char * kItemPaths[] =
{":/images/puzzlemenuitems/exchange_theme*.png",
 ":/images/puzzlemenuitems/unite_theme*.png",
 ":/images/puzzlemenuitems/lock_theme*.png"};

// Pixmaps of the items
QVector<QVector<QPixmap> > puzzleMenuItemPixmaps;

// Total frame count of the items
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
  // Init pixmaps if neccessary
  if (puzzleMenuItemPixmaps.isEmpty())
    initPuzzleMenuItemPixmaps();
  return puzzleMenuItemPixmaps[type][frame % puzzleMenuItemFrameCounts[type]];
}

double AbstractPuzzleMenuItem::width()
{
  return 300;
}

double AbstractPuzzleMenuItem::height()
{
  return 300;
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
  // Get the pixmap
  const QPixmap& pixmap = AbstractPuzzleMenuItem::pixmap(
                            AbstractPuzzleMenuItem::ExchangeItem,
                            frame);

  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  drawPixmapAt(painter,
               pixmap,
               xRate,
               yRate,
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
  // Get the pixmap
  const QPixmap& pixmap = AbstractPuzzleMenuItem::pixmap(
                            AbstractPuzzleMenuItem::UniteItem,
                            frame);

  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  drawPixmapAt(painter,
               pixmap,
               xRate,
               yRate,
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
  // Get the pixmap
  const QPixmap& pixmap = AbstractPuzzleMenuItem::pixmap(
                            AbstractPuzzleMenuItem::LockItem,
                            frame);

  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  drawPixmapAt(painter,
               pixmap,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}
