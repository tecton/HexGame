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
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

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
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }
}

GameOverWidget::~GameOverWidget()
{
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
