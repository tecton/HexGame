#include "achievementwidget.h"

#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include "basicpainter.h"
#include "gamecommonitems.h"
#include "achievements.h"

#define LOGICAL_WIDTH  1024
#define LOGICAL_HEIGHT 600

#define Y_FROM         100.0
#define Y_TO           500.0

AchievementWidget::AchievementWidget() :
    activeAchievementIndex(0)
{
  // Create the items and initialize them
  exitItem = new ButtonItem("Exit");
  exitItem->setPos(QPointF(0.75, 0.9));
  myItems.push_back(exitItem);

  achievementItems = Achievements::getAchievementItems();

  QPointF positions[5];
  positions[0] = QPointF(0.15, 0.2);
  positions[1] = QPointF(0.45, 0.2);
  positions[2] = QPointF(0.15, 0.8);
  positions[3] = QPointF(0.45, 0.8);
  positions[4] = QPointF(0.3, 0.5);

  for (int i = 0;i < achievementItems.size();++i)
  {
    myItems.push_back(achievementItems[i]);
    achievementItems[i]->setPos(positions[i]);
    achievementItems[i]->loseDescriptionFocus();
  }

  // No items was chosen
  itemAtPressPos = NULL;

  t = new QTimer();
  t->setInterval(30);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();
}

void AchievementWidget::makePixmap(
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

void AchievementWidget::makeBasicPixmap(
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

  QRectF rect = QRectF(0.55 * width, 0.1 * height, 0.4 * width, 0.7 * height);
  achievementItems[activeAchievementIndex]->paintDescription(painter, rect, 0);

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void AchievementWidget::addEffect(
#ifdef USE_PIXMAP
      QPixmap& pixmap,
#else
      QPainter* painter,
#endif
                                   int width,
                                   int height){}

QPointF AchievementWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

void AchievementWidget::dealPressed(QPointF mousePos, Qt::MouseButton button)
{
  // Choose the correct item at press position
  if (distanceOfTwoPoints(mousePos,
                          toScene(exitItem->getPos().x(),
                                  exitItem->getPos().y())) < 50)
    itemAtPressPos = exitItem;

  for (int i = 0;i < achievementItems.size();++i)
    if (achievementItems[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      if (activeAchievementIndex != i)
      {
        achievementItems[i]->loseDescriptionFocus();
        activeAchievementIndex = i;
      }
      break;
    }
}

void AchievementWidget::dealMoved(QPointF mousePos, Qt::MouseButton button)
{
  for (int i = 0;i < achievementItems.size();++i)
    if (achievementItems[i]->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
    {
      if (activeAchievementIndex != i)
      {
        achievementItems[i]->loseDescriptionFocus();
        activeAchievementIndex = i;
      }
      break;
    }
}

void AchievementWidget::dealReleased(QPointF mousePos, Qt::MouseButton button)
{
  if (itemAtPressPos == exitItem &&
      exitItem->in(mousePos, LOGICAL_WIDTH, LOGICAL_HEIGHT))
  {
    // Exit
    emit giveControlTo(NULL, true);
    delete this;
    return;
  }

  itemAtPressPos = NULL;
}

void AchievementWidget::advance()
{
  achievementItems[activeAchievementIndex]->advanceDescription();
}

AchievementWidget::~AchievementWidget()
{
  t->stop();
  delete t;
  // Release the space
  for (int i = 0;i < myItems.size();++i)
    delete myItems[i];
}
