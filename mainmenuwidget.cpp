#include "mainmenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "mainmenuitems.h"
#include "swapclassicgame.h"
#include "rotateclassicgame.h"
#include "puzzlemenuwidget.h"

#include <QMessageBox>

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500


MainMenuWidget::MainMenuWidget() :
    frameCount(0)
{
  swapClassicItem = new MainMenuSwapClassicItem();
  swapClassicItem->setPos(QPointF(0.3, 0.5));

  rotateClassicItem = new MainMenuRotateClassicItem();
  rotateClassicItem->setPos(QPointF(0.5, 0.5));

  puzzleMenuItem = new MainMenuRotatePuzzleItem();
  puzzleMenuItem->setPos(QPointF(0.7, 0.5));

  myItems.push_back(swapClassicItem);
  myItems.push_back(rotateClassicItem);
  myItems.push_back(puzzleMenuItem);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void MainMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
//  QMessageBox::critical(0,"","Basic made");
  addEffect(pixmap, width, height);
}

void MainMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  //QMessageBox::critical(0,"","Try to paint items");
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           frameCount);

//  QMessageBox::critical(0,"","Items painted");
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
    SwapClassicGame *swapGame = new SwapClassicGame();
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
