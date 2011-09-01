#include "pausewidget.h"

#include <QPainter>
#include <QPixmap>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "gamemath.h"
#include "othergameinit.h"
#include "abstractrule.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

PauseWidget::PauseWidget()
{
  pauseHint = new StringItem();
  pauseHint->setPos(QPointF(0.5, 0.5));
  pauseHint->setHint("Press anywhere to continue");
  myItems.push_back(pauseHint);
}

void PauseWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

//  void PauseWidget::init();
void PauseWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
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

void PauseWidget::addEffect(QPixmap& pixmap, int width, int height){}

QPointF PauseWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void PauseWidget::dealPressed(QPointF mousePos, Qt::MouseButton button){}

void PauseWidget::dealMoved(QPointF mousePos, Qt::MouseButton button){}

void PauseWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  emit resume();
  emit giveControlTo(NULL, true);
  delete this;
  return;
}

PauseWidget::~PauseWidget()
{
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
