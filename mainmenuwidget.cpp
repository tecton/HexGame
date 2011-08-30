#include "mainmenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "mainmenuitems.h"
#include "swapclassicgame.h"
#include "swaptiminggame.h"
#include "rotateclassicgame.h"
#include "rotatetiminggame.h"
#include "puzzlemenuwidget.h"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500


MainMenuWidget::MainMenuWidget() :
    frameCount(0)
{
  swapClassicItem = new MainMenuSwapClassicItem();
  swapClassicItem->setPos(QPointF(0.2, 0.4));

  rotateClassicItem = new MainMenuRotateClassicItem();
  rotateClassicItem->setPos(QPointF(0.2, 0.6));

  swapEndlessItem = new MainMenuSwapEndlessItem();
  swapEndlessItem->setPos(QPointF(0.4, 0.4));

  rotateEndlessItem = new MainMenuRotateEndlessItem();
  rotateEndlessItem->setPos(QPointF(0.4, 0.6));

  swapTimingItem = new MainMenuSwapTimingItem();
  swapTimingItem->setPos(QPointF(0.6, 0.4));

  rotateTimingItem = new MainMenuRotateTimingItem();
  rotateTimingItem->setPos(QPointF(0.6, 0.6));

  puzzleMenuItem = new MainMenuRotatePuzzleItem();
  puzzleMenuItem->setPos(QPointF(0.8, 0.5));

  myItems.push_back(swapClassicItem);
  myItems.push_back(rotateClassicItem);
  myItems.push_back(swapEndlessItem);
  myItems.push_back(rotateEndlessItem);
  myItems.push_back(swapTimingItem);
  myItems.push_back(rotateTimingItem);
  myItems.push_back(puzzleMenuItem);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void MainMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

void MainMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

  painter->end();
  delete painter;
}

void MainMenuWidget::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF MainMenuWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void MainMenuWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  if (distanceOfTwoPoints(mousePos,
                          toScene(swapClassicItem->getPos().x(),
                                  swapClassicItem->getPos().y())) < 50)
  {
    AbstractPixmapWidget *swapGame = new SwapClassicGame();
    emit giveControlTo(swapGame, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                          toScene(rotateClassicItem->getPos().x(),
                                  rotateClassicItem->getPos().y())) < 50)
  {
    AbstractPixmapWidget *rotateGame = new RotateClassicGame();
    emit giveControlTo(rotateGame, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                          toScene(swapEndlessItem->getPos().x(),
                                  swapEndlessItem->getPos().y())) < 50)
  {
//    AbstractPixmapWidget *swapGame = new SwapEndlessGame();
//    emit giveControlTo(swapGame, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                          toScene(rotateEndlessItem->getPos().x(),
                                  rotateEndlessItem->getPos().y())) < 50)
  {
//    AbstractPixmapWidget *rotateGame = new RotateEndlessGame();
//    emit giveControlTo(rotateGame, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                          toScene(swapTimingItem->getPos().x(),
                                  swapTimingItem->getPos().y())) < 50)
  {
    AbstractPixmapWidget *swapGame = new SwapTimingGame();
    emit giveControlTo(swapGame, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                          toScene(rotateTimingItem->getPos().x(),
                                  rotateTimingItem->getPos().y())) < 50)
  {
    AbstractPixmapWidget *rotateGame = new RotateTimingGame();
    emit giveControlTo(rotateGame, false);
  }
  else if (distanceOfTwoPoints(mousePos,
                          toScene(puzzleMenuItem->getPos().x(),
                                  puzzleMenuItem->getPos().y())) < 50)
  {
    AbstractPixmapWidget *puzzleMenu = new PuzzleMenuWidget();
    emit giveControlTo(puzzleMenu, false);
  }
}

void MainMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{

}

void MainMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{

}

void MainMenuWidget::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  //effectPainter->advance();
}

MainMenuWidget::~MainMenuWidget()
{
  t->stop();
  delete t;
  delete swapClassicItem;
  delete puzzleMenuItem;
}
