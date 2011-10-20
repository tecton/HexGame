#include "gameoverwidget.h"

#include <QPainter>
#include <QPixmap>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "othergameinit.h"
#include "abstractrule.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

GameOverWidget::GameOverWidget(int gameIndex, int score) :
    index(gameIndex)
{
  // Create the items and initialize them
  scoreHint = new IntegerItem(182);
  scoreHint->setPos(QPointF(0.5, 0.3));
  scoreHint->setHint("Your score is");
  scoreHint->setValue(score);
  myItems.push_back(scoreHint);

  newGameHint = new StringItem(316);
  newGameHint->setPos(QPointF(0.5, 0.5));
  newGameHint->setHint("Restart a game?");
  myItems.push_back(newGameHint);

  confirmItem = new ButtonItem("Confirm");
  confirmItem->setPos(QPointF(0.3, 0.7));
  myItems.push_back(confirmItem);

  cancelItem = new ButtonItem("Cancel");
  cancelItem->setPos(QPointF(0.7, 0.7));
  myItems.push_back(cancelItem);

  // No items was chosen
  itemAtPressPos = NULL;
}

void GameOverWidget::makePixmap(
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

void GameOverWidget::makeBasicPixmap(
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

  // Paint the items
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

void GameOverWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height){}

QPointF GameOverWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void GameOverWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
  if (confirmItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    itemAtPressPos = confirmItem;
  else if (cancelItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    itemAtPressPos = cancelItem;
}

void GameOverWidget::dealMoved(QPointF mousePos, Qt::MouseButton button){}

void GameOverWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (itemAtPressPos == confirmItem &&
      confirmItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
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
  else if (itemAtPressPos == cancelItem &&
           cancelItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
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
