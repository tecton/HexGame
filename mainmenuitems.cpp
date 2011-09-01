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

const static int kTotalItems = 9;
const static char * kItemPaths[] = {":/images/mainmenuitems/swapclassicgame*.png",
                                    ":/images/mainmenuitems/rotateclassicgame*.png",
                                    ":/images/mainmenuitems/swapendlessgame*.png",
                                    ":/images/mainmenuitems/rotateendlessgame*.png",
                                    ":/images/mainmenuitems/swaptiminggame*.png",
                                    ":/images/mainmenuitems/rotatetiminggame*.png",
                                    ":/images/mainmenuitems/puzzlegame*.png",
                                    ":/images/mainmenuitems/help*.png",
                                    ":/images/mainmenuitems/exit*.png"};


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

void MainMenuGameItem::paint(QPainter *painter,
                             int width,
                             int height,
                             int frame)
{
  const QPixmap& p = AbstractMainMenuItem::pixmap(type, frame);
  double x = getPos().x() * width;
  double y = getPos().y() * height;
  if (type < 7)
    drawPixmapAt(painter,
                 p,
                 1,
                 1,
                 QPointF(x, y),
                 true,
                 true);
  else
  {
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
}
