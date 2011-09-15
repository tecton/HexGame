#include "mainmenuwidget.h"

#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include "basicpainter.h"
#include "gamemath.h"
#include "mainmenuitems.h"
#include "puzzlemenuwidget.h"
#include "othergameinit.h"
#include "helpwidget.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

MainMenuWidget::MainMenuWidget()/* :
    frameCount(0)*/
{
  // Create the items and initialize them
  items[0] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)0);
  items[0]->setPos(QPointF(0.375, 0.5));
  items[1] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)1);
  items[1]->setPos(QPointF(0.835, 0.5));

  items[2] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)2);
  items[2]->setPos(QPointF(0.49, 0.16));
  items[3] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)3);
  items[3]->setPos(QPointF(0.72, 0.84));

  items[4] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)4);
  items[4]->setPos(QPointF(0.72, 0.16));
  items[5] = new MainMenuGameItem((AbstractMainMenuItem::ItemType)5);
  items[5]->setPos(QPointF(0.49, 0.84));

  items[6] = new MainMenuGameItem(AbstractMainMenuItem::RotatePuzzleItem);
  items[6]->setPos(QPointF(0.605, 0.5));

  items[7] = new MainMenuGameItem(AbstractMainMenuItem::HelpItem);
  items[7]->setPos(QPointF(0.1, 0.7));

  items[8] = new MainMenuGameItem(AbstractMainMenuItem::ExitItem);
  items[8]->setPos(QPointF(0.1, 0.8));

  for (int i = 0;i < 9;++i)
    myItems.push_back(items[i]);

//  // Create the timer and connect signals and slots
//  t = new QTimer();
//  t->setInterval(75);
//  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
//  t->start();
}

void MainMenuWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

void MainMenuWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);

  // Paint the background
  BasicPainter::paintBackGround(BasicPainter::MainMenu,
                                painter,
                                width,
                                height,
                                0/*frameCount*/);

  // Paint the basic balls
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0/*frameCount*/);

  // End the paint and release the space
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
  // Quit if it's a right button. May be abandoned later
  if (button == Qt::RightButton)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  // Create correct game if neccessary
  for (int i = 0;i < 6;++i)
  {
    if (distanceOfTwoPoints(mousePos,
                            toScene(items[i]->getPos().x(),
                                    items[i]->getPos().y())) < 80)
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

  // Create puzzle game if neccessary
  if (distanceOfTwoPoints(mousePos,
                          toScene(items[6]->getPos().x(),
                                  items[6]->getPos().y())) < 50)
  {
    AbstractPixmapWidget *puzzleMenu = new PuzzleMenuWidget();
    emit giveControlTo(puzzleMenu, false);
    return;
  }
  // Go to help if neccessary
  else if (distanceOfTwoPoints(mousePos,
                          toScene(items[7]->getPos().x(),
                                  items[7]->getPos().y())) < 50)
  {
    AbstractPixmapWidget *helpWidget = new HelpWidget();
    emit giveControlTo(helpWidget, false);
    return;
  }
  // Exit if neccessary
  else if (distanceOfTwoPoints(mousePos,
                          toScene(items[8]->getPos().x(),
                                  items[8]->getPos().y())) < 50)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }


}

void MainMenuWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
}

void MainMenuWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
}

//void MainMenuWidget::advance()
//{
//  ++frameCount;
//  frameCount = frameCount;
//}

MainMenuWidget::~MainMenuWidget()
{
//  // Stop the timer
//  t->stop();
  // Release the space
//  delete t;
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
  myItems.clear();
}
