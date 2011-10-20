#include "mainmenuitems.h"

#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QPainter>
#include "pixmapoperations.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

#define BUTTON_LOGICAL_WIDTH  120
#define BUTTON_LOGICAL_HEIGHT 60

// Total color number of the items
const static int kTotalItems = 10;

// File path of the items
// (The last two are the same as two before them, may make
//  cause some bug later)
const static char * kItemPaths[] =
{":/images/mainmenuitems/swapclassicgame*.png",
 ":/images/mainmenuitems/rotateclassicgame*.png",
 ":/images/mainmenuitems/swapendlessgame*.png",
 ":/images/mainmenuitems/rotateendlessgame*.png",
 ":/images/mainmenuitems/swaptiminggame*.png",
 ":/images/mainmenuitems/rotatetiminggame*.png",
 ":/images/mainmenuitems/puzzlegame*.png",
 ":/images/mainmenuitems/help*.png",
 ":/images/mainmenuitems/exit*.png",
 ":/images/mainmenuitems/swapclassicgame*.png"};

// Pixmaps of the items
QVector<QVector<QPixmap> > mainMenuItemPixmaps;

// Total frame count of the items
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
  return mainMenuItemPixmaps[type][frame % mainMenuItemFrameCounts[type]];
}

MainMenuGameItem::MainMenuGameItem(AbstractMainMenuItem::ItemType theType) :
    type(theType)
{
  // Init pixmaps if neccessary
  if (mainMenuItemPixmaps.isEmpty())
    initMainMenuItemPixmaps();
}

void MainMenuGameItem::paint(QPainter *painter,
                             int width,
                             int height,
                             int frame)
{
  // Init pixmaps if neccessary
  if (mainMenuItemPixmaps.isEmpty())
    initMainMenuItemPixmaps();
  if (type == AbstractMainMenuItem::UnvisibleBig)
    return;

  // Get the pixmap
  const QPixmap& p = AbstractMainMenuItem::pixmap(type, frame);

  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

double MainMenuGameItem::r()
{
  if (type != AbstractMainMenuItem::UnvisibleBig)
    return 80;
  return 120;
}


void MainMenuButtonItem::paint(QPainter *painter,
                               int width,
                               int height,
                               int frame)
{
  // Get the pixmap
  const QPixmap& p = AbstractMainMenuItem::pixmap(type, frame);

  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width * BUTTON_LOGICAL_WIDTH / LOGICAL_WIDTH /
                 p.width();
  double yRate = 1.0 * height * BUTTON_LOGICAL_HEIGHT / LOGICAL_HEIGHT /
                 p.height();

  // draw the pixmap
  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(x, y),
               true,
               true);
}

double MainMenuButtonItem::width()
{
  return BUTTON_LOGICAL_WIDTH;
}

double MainMenuButtonItem::height()
{
  return BUTTON_LOGICAL_HEIGHT;
}

RotatingCircleItem::RotatingCircleItem()
{
  p = QPixmap(":/images/mainmenuitems/circle.png");
}

void RotatingCircleItem::paint(QPainter *painter,
                               int width,
                               int height,
                               int frame)
{
  // Calculate the values to locate
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  double xRate = 1.0 * width  / LOGICAL_WIDTH;
  double yRate = 1.0 * height / LOGICAL_HEIGHT;

  // draw the pixmap
  painter->translate(x, y);
  painter->rotate(- frame * 3);

  drawPixmapAt(painter,
               p,
               xRate,
               yRate,
               QPointF(0, 0),
               true,
               true);

  painter->rotate(frame * 3);
  painter->translate(-x, -y);
}
