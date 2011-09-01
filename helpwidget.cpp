#include "helpwidget.h"

#include <QPainter>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "gamemath.h"
#include "othergameinit.h"
#include "abstractrule.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

HelpWidget::HelpWidget() :
    dy(0)
{
  p = QPixmap(":/images/helpwidget/helpwidget.png");

  exitItem = new ExitItem();
  exitItem->setPos(QPointF(0.5, 0.95));
  myItems.push_back(exitItem);

  itemAtPressPos = NULL;
}

void HelpWidget::makePixmap(QPixmap& pixmap, int width, int height)
{
  makeBasicPixmap(pixmap, width, height);
  addEffect(pixmap, width, height);
}

//  void HelpWidget::init();
void HelpWidget::makeBasicPixmap(QPixmap& pixmap, int width, int height)
{
  pixmap = QPixmap(width, height);
  pixmap.fill(Qt::black);
  QPainter *painter = new QPainter(&pixmap);
  double scale;
  double picXYRate = 1.0 * p.width() / p.height();
  double xyRate = 1.0 * width / height;

  scale = 1.0 * width / p.width();
  dy = qMax(qMin(dy, p.height() * scale - height), 0.0);
  painter->drawPixmap(0,
                      -dy,
                      width,
                      p.height() * scale,
                      p
                      );

  BasicPainter::paintItems(painter,
                           myItems,
                           width,
                           height,
                           0);

  painter->end();
  delete painter;
}

void HelpWidget::addEffect(QPixmap& pixmap, int width, int height){}

QPointF HelpWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void HelpWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  lastPos = mousePos;
  if (distanceOfTwoPoints(mousePos,
                          toScene(exitItem->getPos().x(),
                                  exitItem->getPos().y())) < 50)
    itemAtPressPos = exitItem;
}

void HelpWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
//  if ((button & Qt::LeftButton) == Qt::LeftButton)
    dy = dy + lastPos.y() - mousePos.y();

  lastPos = mousePos;
}

void HelpWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (distanceOfTwoPoints(mousePos,
                          toScene(exitItem->getPos().x(),
                                  exitItem->getPos().y())) < 50 &&
      itemAtPressPos == exitItem)
  {
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  itemAtPressPos = NULL;
}

HelpWidget::~HelpWidget()
{
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
