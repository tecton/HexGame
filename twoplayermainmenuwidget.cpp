#include "twoplayermainmenuwidget.h"

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
#include "twoplayertiminggamewidget2.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

TwoPlayerMainMenuWidget::TwoPlayerMainMenuWidget() :
    frameCount(0)
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

  swapTimingItem = new MainMenuGameItem(AbstractMainMenuItem::SwapTimingItem);
  swapTimingItem->setPos(QPointF(0.375, 0.5));
  myItems.push_back(swapTimingItem);

  rotateTimingItem = new MainMenuGameItem(AbstractMainMenuItem::RotateTimingItem);
  rotateTimingItem->setPos(QPointF(0.835, 0.5));
  myItems.push_back(rotateTimingItem);

  AbstractItem *board = new RotatingCircleItem(true);
  board->setPos(QPointF(0.6, 0.495));
  myItems.push_back(board);

  AbstractItem *boardCenter = new MainMenuGameItem(AbstractMainMenuItem::TwoPlayerGame);
  boardCenter->setPos(QPointF(0.605, 0.5));
  myItems.push_back(boardCenter);

  // Create the timer and connect signals and slots
  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void TwoPlayerMainMenuWidget::makePixmap(
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

void TwoPlayerMainMenuWidget::makeBasicPixmap(
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
  BasicPainter::paintBackGround(BasicPainter::MainMenu,
                                painter,
                                width,
                                height,
                                frameCount);

  // Paint the basic balls
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void TwoPlayerMainMenuWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                               int width,
                               int height)
{
}

QPointF TwoPlayerMainMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void TwoPlayerMainMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
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
  else if (swapTimingItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractPixmapWidget *game = new TwoPlayerTimingGameWidget2(AbstractRule::Swap);
    emit giveControlTo(game, false);
    return;
  }
  // Two players timing game
  else if (rotateTimingItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    AbstractPixmapWidget *game = new TwoPlayerTimingGameWidget2(AbstractRule::Rotate);
    emit giveControlTo(game, false);
    return;
  }
}

void TwoPlayerMainMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
}

void TwoPlayerMainMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
}

void TwoPlayerMainMenuWidget::advance()
{
  ++frameCount;
  frameCount = frameCount;
}

TwoPlayerMainMenuWidget::~TwoPlayerMainMenuWidget()
{
  // Stop the timer
  t->stop();
  // Release the space
  delete t;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  myItems.clear();
}
