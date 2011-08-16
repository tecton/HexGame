#include "puzzlemenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>

#include <QMessageBox>

const static int kTotalItems = 3;
const static char * kItemPaths[] = {":/images/mainmenuitems/exchange_theme*.png",
                                   ":/images/mainmenuitems/unite_theme*.png",
                                   ":/images/mainmenuitems/lock_theme*.png"};


QVector<QVector<QPixmap> > puzzleMenuItemPixmaps;
QVector<int> puzzleMenuItemFrameCounts;
void initPuzzleMenuItemPixmaps()
{
  puzzleMenuItemPixmaps.reserve(kTotalItems);
  puzzleMenuItemFrameCounts.reserve(kTotalItems);
  for (int i = 0;i < kTotalItems;++i)
  {
    int count = 0;
    QVector<QPixmap> current;
    QString path(kItemPaths[i]);
    QFileInfo fi(path);
    // For each file of the color
    foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
    {
      // Get the pixmap of the path
      current << QPixmap(fi.path() + "/" + entry);
      ++count;
    }
//    QMessageBox::critical(0,"","init pixmaps");
    puzzleMenuItemPixmaps << current;
    puzzleMenuItemFrameCounts << count;
  }
//  QMessageBox::critical(0,"","init pixmaps");
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
  painter->drawPixmap(QPointF(x, y), pixmap);
//  QMessageBox::critical(0,"","Painted one item");
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
  painter->drawPixmap(QPointF(x, y), pixmap);
//  QMessageBox::critical(0,"","Painted one item");
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
  painter->drawPixmap(QPointF(x, y), pixmap);
//  QMessageBox::critical(0,"","Painted one item");
}
