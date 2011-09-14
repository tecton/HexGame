#include "gameoverwidget.h"

#include <QPainter>
#include <QPixmap>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "gamemath.h"
#include "othergameinit.h"
#include "abstractrule.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

GameOverWidget::GameOverWidget(int gameIndex, int score) :
    index(gameIndex)
{
  // Create the items and initialize them
  scoreHint = new IntegerItem();
  scoreHint->setPos(QPointF(0.5, 0.3));
  scoreHint->setHint("Your score is");
  scoreHint->setValue(score);
  myItems.push_back(scoreHint);

  newGameHint = new StringItem();
  newGameHint->setPos(QPointF(0.5, 0.5));
  newGameHint->setHint("Restart a game?");
  myItems.push_back(newGameHint);

  confirmItem = new ConfirmItem();
  confirmItem->setPos(QPointF(0.3, 0.7));
  myItems.push_back(confirmItem);

  cancelItem = new CancelItem();
  cancelItem->setPos(QPointF(0.7, 0.7));
  myItems.push_back(cancelItem);

  // No items was chosen
  itemAtPressPos = NULL;
}

void GameOverWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

//  void GameOverWidget::init();
void GameOverWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);

  // Fill the pixmap with black background
  pixmap.fill(Qt::black);

  // Get the painter
  QPainter *painter = new QPainter(&pixmap);

  // Paint the items
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

  // End the paint and release the space
  painter->end();
  delete painter;
}

void GameOverWidget::addEffect(QPixmap& pixmap, int width, int height){}

QPointF GameOverWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void GameOverWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
  if (distanceOfTwoPoints(mousePos,
                          toScene(confirmItem->getPos().x(),
                                  confirmItem->getPos().y())) < 50)
    itemAtPressPos = confirmItem;
  else if (distanceOfTwoPoints(mousePos,
                               toScene(cancelItem->getPos().x(),
                                       cancelItem->getPos().y())) < 50)
    itemAtPressPos = cancelItem;
}

void GameOverWidget::dealMoved(QPointF mousePos, Qt::MouseButton button){}

void GameOverWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (distanceOfTwoPoints(mousePos,
                          toScene(confirmItem->getPos().x(),
                                  confirmItem->getPos().y())) < 50 &&
      itemAtPressPos == confirmItem)
  {
    // Start a new game
    AbstractRule::Gesture g = (index % 2 == 0) ?
                              AbstractRule::Swap :
                              AbstractRule::Rotate;
    AbstractPixmapWidget *w = OtherGameInit::initOtherGame(g, index / 2);
    emit giveControlTo(w, true);
    delete this;
    return;
  }
  else if (distanceOfTwoPoints(mousePos,
                               toScene(cancelItem->getPos().x(),
                                       cancelItem->getPos().y())) < 50 &&
           itemAtPressPos == cancelItem)
  {
    // Exit
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
}

GameOverWidget::~GameOverWidget()
{
  // Release the space
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
