#include "mainmenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "mainmenuitems.h"
#include "puzzlemenuwidget.h"
#include "othergameinit.h"

#define LOGICAL_WIDTH  800
#define LOGICAL_HEIGHT 500


MainMenuWidget::MainMenuWidget() :
    frameCount(0)
{
  for (int i = 0;i < 6;++i)
  {
    items[i] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)i);
    items[i]->setPos(QPointF((i / 2) * 0.2 + 0.2,
                             (i % 2) * 0.2 + 0.4));
    myItems.push_back(items[i]);
  }

  items[6] = new MainMenuGameItem(AbstractMainMenuItem::RotatePuzzleItem);
  items[6]->setPos(QPointF(0.8, 0.5));

  myItems.push_back(items[6]);

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
  for (int i = 0;i < 6;++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            toScene(items[i]->getPos().x(),
                                    items[i]->getPos().y())) < 50)
    {
      AbstractRule::Gesture gesture = (i % 2 == 0) ?
                                      AbstractRule::Swap :
                                      AbstractRule::Rotate;
      AbstractPixmapWidget *game = OtherGameInit::initOtherGame(gesture,
                                                                i / 2);
      emit giveControlTo(game, false);
      return;
    }
  }

  if (distanceOfTwoPoints(mousePos,
                          toScene(items[6]->getPos().x(),
                                  items[6]->getPos().y())) < 50)
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
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
