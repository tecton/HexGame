#include "mainmenu.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "mainmenuitems.h"
#include "swapclassicgame.h"

#include <QMessageBox>

#define MAIN_MENU_LOGICL_WIDTH  800
#define MAIN_MENU_LOGICL_HEIGHT 500


MainMenu::MainMenu() :
    frameCount(0)
{
  swapClassicItem = new MainMenuSwapClassicGameItem();
  swapClassicItem->setPos(QPointF(0.5, 0.5));

  myItems.push_back(swapClassicItem);

  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void MainMenu::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
//  QMessageBox::critical(0,"","Basic made");
  addEffect(pixmap, width, height);
}

void MainMenu::makeBasicPixmap(QPixmap& pixmap, int width, int height)
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

void MainMenu::addEffect(QPixmap& pixmap, int width, int height)
{

}

QPointF MainMenu::toScene(double xRate, double yRate)
{
  return QPointF(xRate * MAIN_MENU_LOGICL_WIDTH,
                 yRate * MAIN_MENU_LOGICL_HEIGHT);
}

void MainMenu::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
  if (distanceOfTwoPoints(mousePos,
                          QPointF(0.5 * MAIN_MENU_LOGICL_WIDTH,
                                  0.5 * MAIN_MENU_LOGICL_HEIGHT)) < 100)
  {
    SwapClassicGame *swapGame = new SwapClassicGame();
    emit giveControlTo(swapGame, false);
  }
}

void MainMenu::dealMoved(QPointF mousePos, Qt::MouseButton button)
{

}

void MainMenu::dealReleased(QPointF mousePos, Qt::MouseButton button)
{

}

void MainMenu::advance()
{
  ++frameCount;
  frameCount = frameCount % 32;
  //effectPainter->advance();
}

MainMenu::~MainMenu()
{
  t->stop();
  delete t;
  delete swapClassicItem;
}
