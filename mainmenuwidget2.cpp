#include "mainmenuwidget2.h"


#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "achievementwidget.h"
#include "basicpainter.h"
#include "mainmenuitems.h"
#include "gamecommonitems.h"
#include "puzzlemenuwidget.h"
#include "othergameinit.h"
#include "helpwidget2.h"
#include "mainmenuwidget.h"
#include "twoplayermainmenuwidget.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

MainMenuWidget2::MainMenuWidget2()
{
  achievementItem = new ButtonItem("Achievement");
  achievementItem->setPos(QPointF(0.1, 0.55));
  myItems.push_back(achievementItem);

  helpItem = new ButtonItem("Help");
  helpItem->setPos(QPointF(0.1, 0.65));
  myItems.push_back(helpItem);

  exitItem = new ButtonItem("Exit");
  exitItem->setPos(QPointF(0.1, 0.8));
  myItems.push_back(exitItem);

  singleItem = new MainMenuGameItem(AbstractMainMenuItem::UnvisibleBig);
  singleItem->setPos(QPointF(0.572, 0.4));
  myItems.push_back(singleItem);

  multipleItem = new MainMenuGameItem(AbstractMainMenuItem::UnvisibleBig);
  multipleItem->setPos(QPointF(0.784, 0.69));
  myItems.push_back(multipleItem);
}

void MainMenuWidget2::makePixmap(
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

void MainMenuWidget2::makeBasicPixmap(
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
#endif

  // Paint the background
  BasicPainter::paintBackGround(BasicPainter::MainMenu2,
                                painter,
                                width,
                                height,
                                0);

  // Paint the basic balls
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void MainMenuWidget2::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height)
{
}

QPointF MainMenuWidget2::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void MainMenuWidget2::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Quit if it's a right button. May be abandoned later
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  // Create puzzle game if neccessary
  if (helpItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractPixmapWidget *helpWidget = new HelpWidget2();
    emit giveControlTo(helpWidget, false);
    return;
  }
  // Go to achievement if neccessary
  else if (achievementItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractPixmapWidget *achievementWidget = new AchievementWidget();
    emit giveControlTo(achievementWidget, false);
    return;
  }
  // Go to help if neccessary
  else if (exitItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  // Exit if neccessary
  else if (singleItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractPixmapWidget *menu = new MainMenuWidget();
    emit giveControlTo(menu, false);
    return;
  }
  // Two players timing game
  else if (multipleItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
   AbstractPixmapWidget *menu = new TwoPlayerMainMenuWidget();
   emit giveControlTo(menu, false);
   return;
  }
}

void MainMenuWidget2::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
}

void MainMenuWidget2::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
}

MainMenuWidget2::~MainMenuWidget2()
{
//  // Stop the timer
//  t->stop();
  // Release the space
//  delete t;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  myItems.clear();
}
