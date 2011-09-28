#include "helpwidget2.h"

#include <QPainter>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "helpmainpartwidget.h"
#include "helpitem.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

#define MAIN_X         200
#define MAIN_Y         0
#define MAIN_WIDTH     800
#define MAIN_HEIGHT    600

#define VIEW_X         (-MAIN_X)
#define VIEW_Y         (-MAIN_Y)
#define VIEW_WIDTH     LOGICAL_WIDTH
#define VIEW_HEIGHT    LOGICAL_HEIGHT

#define ITEM_Y_FROM    100.0
#define ITEM_Y_TO      500.0

HelpWidget2::HelpWidget2() :
  currentItemIndex(0),
  mainPartFrameCount(0)
{
  // Create the items and initialize them
  exitItem = new HelpItem(HelpItem::totalItems());
  exitItem->setPos(QPointF(0.113, 0.92));
  myItems.push_back(exitItem);

  QPointF positions[6];
  positions[0] = QPointF(0.113, 0.09);
  positions[1] = QPointF(0.113, 0.218);
  positions[2] = QPointF(0.113, 0.358);
  positions[3] = QPointF(0.113, 0.494);
  positions[4] = QPointF(0.113, 0.63);
  positions[5] = QPointF(0.113, 0.778);

  helpThemeItems = new HelpItem *[HelpItem::totalItems()];
  helpThemeWidgets = new HelpMainPartWidget *[HelpItem::totalItems()];

  for (int i = 0;i < HelpItem::totalItems();++i)
  {
    helpThemeItems[i] = new HelpItem(i);
    helpThemeItems[i]->setPos(positions[i]);

    helpThemeWidgets[i] = new HelpMainPartWidget(
        helpThemeItems[i]->mainPartBackGroundPath(),
        helpThemeItems[i]->mainPartAnimCenters(),
        helpThemeItems[i]->mainPartAnimPaths());

    myItems.push_back(helpThemeItems[i]);
  }

  // No items was chosen
  itemAtPressPos = NULL;
}

void HelpWidget2::makePixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
      makeBasicPixmap(pixmap, width, height);
      addEffect(pixmap, width, height);
#else
      makeBasicPixmap(painter, width, height);
      addEffect(painter, width, height);
#endif
}

void HelpWidget2::makeBasicPixmap(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height)
{
#ifdef USE_PIXMAP
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);
#else
  painter->fillRect(0,0,width,height,QColor(0,0,0));
#endif

  BasicPainter::paintBackGround(BasicPainter::Help, painter, width, height, 0);

  // Paint the items
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

  if (HelpItem::totalItems() > 0)
  {
    painter->setWindow(VIEW_X, VIEW_Y, VIEW_WIDTH, VIEW_HEIGHT);
    helpThemeWidgets[currentItemIndex]->paint(painter,
                                              MAIN_WIDTH,
                                              MAIN_HEIGHT,
                                              mainPartFrameCount);
  }

  ++mainPartFrameCount;

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void HelpWidget2::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height){}

QPointF HelpWidget2::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void HelpWidget2::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
  if (distanceOfTwoPoints(mousePos,
                          toScene(exitItem->getPos().x(),
                                  exitItem->getPos().y())) < 50)
    itemAtPressPos = exitItem;

  for (int i = 0;i < HelpItem::totalItems();++i)
    if (helpThemeItems[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      if (currentItemIndex != i)
      {
        mainPartFrameCount = 0;
        currentItemIndex = i;
      }
      break;
    }
}

void HelpWidget2::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  for (int i = 0;i < HelpItem::totalItems();++i)
    if (helpThemeItems[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      if (currentItemIndex != i)
      {
        mainPartFrameCount = 0;
        currentItemIndex = i;
      }
      break;
    }
}

void HelpWidget2::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (itemAtPressPos == exitItem &&
      exitItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    // Exit
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  itemAtPressPos = NULL;
}

HelpWidget2::~HelpWidget2()
{
  // Release the space
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];

  for (int i = 0;i < HelpItem::totalItems();++i)
    delete helpThemeWidgets[i];

  delete [] helpThemeItems;
  delete [] helpThemeWidgets;
}
