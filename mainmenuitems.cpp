#include "mainmenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>

#include <QMessageBox>

const static int kTotalItems = 2;
const static char * kItemPaths[] = {":/images/mainmenuitems/swapclassicgame*.png",
                                    ":/images/mainmenuitems/puzzlegame*.png"};


QVector<QVector<QPixmap> > mainMenuItemPixmaps;
QVector<int> mainMenuItemFrameCounts;
void initMainMenuItemPixmaps()
{
  mainMenuItemPixmaps.reserve(kTotalItems);
  mainMenuItemFrameCounts.reserve(kTotalItems);
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
    mainMenuItemPixmaps << current;
    mainMenuItemFrameCounts << count;
  }
//  QMessageBox::critical(0,"","init pixmaps");
}

const QPixmap& AbstractMainMenuItem::pixmap(ItemType type,
                                            int frame)
{
  if (mainMenuItemPixmaps.isEmpty())
    initMainMenuItemPixmaps();
  return mainMenuItemPixmaps[type][frame % mainMenuItemFrameCounts[type]];
}

MainMenuSwapClassicItem::MainMenuSwapClassicItem()
{
  setPos(QPointF(0, 0));
}

void MainMenuSwapClassicItem::paint(QPainter *painter,
                                        int width,
                                        int height,
                                        int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::SwapClassicItem,
                            frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawPixmap(QPointF(x, y), pixmap);
//  QMessageBox::critical(0,"","Painted one item");
}

MainMenuRotatePuzzleItem::MainMenuRotatePuzzleItem()
{
  setPos(QPointF(0, 0));
}

void MainMenuRotatePuzzleItem::paint(QPainter *painter,
                                        int width,
                                        int height,
                                        int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::RotatePuzzleItem,
                            frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  painter->drawPixmap(QPointF(x, y), pixmap);
//  QMessageBox::critical(0,"","Painted one item");
}
