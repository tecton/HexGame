#include "helpitem.h"

#include <QPainter>
#include "pixmapoperations.h"
#include "gamemath.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

#define BUTTON_LOGICAL_WIDTH  190
#define BUTTON_LOGICAL_HEIGHT 50

const static int helpItemCount = 6;

QVector<QString> mainPartBackGroundPaths;
QVector<QVector<QPointF> > mainPartAnimCenterss;
QVector<QVector<QString> > mainPartAnimPathss;

void initThingsAboutmainPart()
{
  mainPartBackGroundPaths.push_back(":/images/help/mode.png");
  mainPartAnimCenterss.push_back(QVector<QPointF>());
  mainPartAnimPathss.push_back(QVector<QString>());
  mainPartBackGroundPaths.push_back(":/images/help/gesture.png");
  mainPartAnimCenterss.push_back(QVector<QPointF>());
  mainPartAnimPathss.push_back(QVector<QString>());
  mainPartBackGroundPaths.push_back(":/images/help/score.png");
  mainPartAnimCenterss.push_back(QVector<QPointF>());
  mainPartAnimPathss.push_back(QVector<QString>());
  mainPartBackGroundPaths.push_back(":/images/help/items.png");
  mainPartAnimCenterss.push_back(QVector<QPointF>());
  mainPartAnimPathss.push_back(QVector<QString>());
  mainPartBackGroundPaths.push_back(":/images/help/puzzle.png");
  mainPartAnimCenterss.push_back(QVector<QPointF>());
  mainPartAnimPathss.push_back(QVector<QString>());
  mainPartBackGroundPaths.push_back(":/images/help/about.png");
  mainPartAnimCenterss.push_back(QVector<QPointF>());
  mainPartAnimPathss.push_back(QVector<QString>());
}

int HelpItem::totalItems()
{
  return helpItemCount;
}

HelpItem::HelpItem(int theIndex) :
    index(theIndex)
{
  setPos(QPointF(0, 0));
  if (mainPartBackGroundPaths.isEmpty() && index < helpItemCount)
    initThingsAboutmainPart();
}

void HelpItem::paint(QPainter *painter, int width, int height, int frame)
{
//  QPointF center = QPointF(getPos().x() * width,
//                           getPos().y() * height);
//  double xRate = 1.0 * width / LOGICAL_WIDTH;
//  double yRate = 1.0 * height / LOGICAL_HEIGHT;
//  painter->drawRect(center.x() - BUTTON_LOGICAL_WIDTH / 2 * xRate,
//                    center.y() - BUTTON_LOGICAL_HEIGHT / 2 * yRate,
//                    BUTTON_LOGICAL_WIDTH * xRate,
//                    BUTTON_LOGICAL_HEIGHT * yRate);
}

double HelpItem::width()
{
  return BUTTON_LOGICAL_WIDTH;
}

double HelpItem::height()
{
  return BUTTON_LOGICAL_HEIGHT;
}


QString HelpItem::mainPartBackGroundPath()
{
  return mainPartBackGroundPaths[index];
}

QVector<QPointF> HelpItem::mainPartAnimCenters()
{
  return mainPartAnimCenterss[index];
}

QVector<QString> HelpItem::mainPartAnimPaths()
{
  return mainPartAnimPathss[index];
}
