#include "mainmenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

const static int kTotalItems = 3;
const static char * kItemPaths[] = {":/images/mainmenuitems/swapclassicgame*.png",
                                    ":/images/mainmenuitems/rotateclassicgame*.png",
                                    ":/images/mainmenuitems/puzzlegame*.png"};


QVector<QVector<QPixmap> > mainMenuItemPixmaps;
QVector<int> mainMenuItemFrameCounts;
void initMainMenuItemPixmaps()
{
  initPixmaps(kTotalItems,
              kItemPaths,
              mainMenuItemPixmaps,
              mainMenuItemFrameCounts);
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
  drawPixmapAt(painter,
               pixmap,
               1,
               1,
               QPointF(x, y),
               false,
               true);
}

MainMenuRotateClassicItem::MainMenuRotateClassicItem()
{
  setPos(QPointF(0, 0));
}

void MainMenuRotateClassicItem::paint(QPainter *painter,
                                      int width,
                                      int height,
                                      int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::RotateClassicItem,
                            frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  drawPixmapAt(painter,
               pixmap,
               1,
               1,
               QPointF(x, y),
               false,
               true);
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
  drawPixmapAt(painter,
               pixmap,
               1,
               1,
               QPointF(x, y),
               false,
               true);
}
