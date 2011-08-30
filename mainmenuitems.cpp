#include "mainmenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

const static int kTotalItems = 7;
const static char * kItemPaths[] = {":/images/mainmenuitems/swapclassicgame*.png",
                                    ":/images/mainmenuitems/rotateclassicgame*.png",
                                    ":/images/mainmenuitems/swapendlessgame*.png",
                                    ":/images/mainmenuitems/rotateendlessgame*.png",
                                    ":/images/mainmenuitems/swaptiminggame*.png",
                                    ":/images/mainmenuitems/rotatetiminggame*.png",
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

void MainMenuSwapEndlessItem::paint(QPainter *painter,
                                    int width,
                                    int height,
                                    int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::SwapEndlessItem,
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

void MainMenuRotateEndlessItem::paint(QPainter *painter,
                                      int width,
                                      int height,
                                      int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::RotateEndlessItem,
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

void MainMenuSwapTimingItem::paint(QPainter *painter,
                                   int width,
                                   int height,
                                   int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::SwapTimingItem,
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

void MainMenuRotateTimingItem::paint(QPainter *painter,
                                     int width,
                                     int height,
                                     int frame)
{
  const QPixmap& pixmap = AbstractMainMenuItem::pixmap(
                            AbstractMainMenuItem::RotateTimingItem,
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
