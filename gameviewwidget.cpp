#include <QGridLayout>
#include <QMessageBox>
#include <QBrush>
#include "gameviewwidget.h"
#include "gamelocation.h"

GameViewWidget::GameViewWidget(QWidget *parent) :
    QWidget(parent)
{
  scene = new QGraphicsScene();
  scene->setSceneRect(LOCATION_GAME_VIEW_X_FROM,
                      LOCATION_GAME_VIEW_Y_FROM,
                      LOCATION_GAME_VIEW_X_TO,
                      LOCATION_GAME_VIEW_Y_TO);
  view = new QGraphicsView(scene);

  view->setFocusPolicy( Qt::StrongFocus );
  view->setBackgroundBrush(QBrush(QColor(0,0,0)));
  QGridLayout *layout = new QGridLayout();
  layout->addWidget(view);
  setLayout(layout);

  balls = new PixmapItem*[TOTAL_ITEM_NUMBER];
  for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
  {
    balls[i] = NULL;
//    balls[i] = new PixmapItem((PixmapItem::BALL_COLOR)(rand() % 6), 0);
//    balls[i]->setPos(positionOfIndex(i));
//    scene->addItem(balls[i]);
  }

  fillAllBlanks();
//  PixmapItem *a = new PixmapItem();
//  a->setPos(positionOfIndex(30));
//  scene->addItem(a);
//  PixmapItem *b = new PixmapItem();
//  b->setPos(positionOfIndex(60));
//  scene->addItem(b);

//  tmp = new PixmapItem();
//  tmp->setPos(positionOfIndex(56));
//  scene->addItem(tmp);
//  tmp->rotateTo(31, 60, PixmapItem::STABLE);



  t = new QTimer();
  t->setInterval(1000/REFRESH_PER_SEC);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();

}

void GameViewWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
      QPointF p = view->mapToScene(event->pos());
      int index = indexOfPosition(p);
//      QMessageBox::critical(0,"",QString::number(index));
      if (index < 0 || index >= TOTAL_ITEM_NUMBER)
        return;
      first = index;
//      if (balls[index])
//      {
//        balls[index]->recycle();
//        balls[index] = NULL;
//      }
    }
//    if (event->button() == Qt::RightButton)
//    {
//      fillAllBlanks();
//    }
}

void GameViewWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
      QPointF p = view->mapToScene(event->pos());
      int index = indexOfPosition(p);
      QMessageBox::critical(0,"",QString::number(index));
      if (index < 0 || index >= TOTAL_ITEM_NUMBER)
        return;
      int left = leftIndex(first);
      int right = rightIndex(first);
      if (index == left)
          direction = 1;
      else if (index == right)
          direction = 2;
      else
      {
          if (direction == 0)
              return;
          else if ((direction == 1 && index == leftIndex(left))
                   || (direction == 2 && index == rightIndex(right)))
              QMessageBox::critical(0,"",QString::number(index));
      }
    }
}

void GameViewWidget::mouseReleaseEvent(QMouseEvent *event)
{
//  QVector<int> chain = doubleChain(chainOfIndex(18, true));
//  tmp->rotateWithChain(chain, 18, 26, PixmapItem::STABLE);
//  tmp->recycle();
//  tmp->translateTo(39, PixmapItem::STABLE, 20, false);
//  tmp->rotateTo(31, 60, PixmapItem::STABLE);
//  QPointF tmp = calculatePosition(-PI/8,100,QPointF(0,0));
//  QMessageBox::critical(0,"",QString::number(tmp.x()) +
//                             "   " +
//                             QString::number(tmp.y()));
  if (event->button() == Qt::LeftButton)
  {
    QPointF p = view->mapToScene(event->pos());
    int index = indexOfPosition(p);
//    QMessageBox::critical(0,"",QString::number(index));
    if (index < 0 || index >= TOTAL_ITEM_NUMBER)
      return;
    if (balls[index])
    {
      balls[index]->recycle();
      balls[index] = NULL;
    }
  }
  if (event->button() == Qt::RightButton)
  {
    fillAllBlanks();
  }
}

void GameViewWidget::advance()
{
  scene->advance();
}

void GameViewWidget::fillAllBlanks(bool allowMoreMove,
                                   bool withoutDirectElimination)
{
  autoRotate();
  QList <int> blankIndexes;
  for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
    if (!balls[i])
      blankIndexes.push_back(i);
  do
  {
    for (QList <int>::Iterator itr = blankIndexes.begin();
         itr != blankIndexes.end();
         ++itr)
    {
      if (!balls[*itr])
        balls[*itr] = new PixmapItem((PixmapItem::BALL_COLOR)(rand() % 6));
      else
        balls[*itr]->setColor((PixmapItem::BALL_COLOR)(rand() % 6));
//      balls[*itr]->hide();

    }
  } while (withoutDirectElimination /* && 已有可消去*/); // 这里面的要好好想想

  for (QList <int>::Iterator itr = blankIndexes.begin();
       itr != blankIndexes.end();
       ++itr)
  {
    balls[*itr]->setPos(positionOfIndex(*itr));
//    balls[*itr]->show();
    scene->addItem(balls[*itr]);
  }

  autoRotate();

}

void GameViewWidget::autoRotate()
{
  const QVector<QVector<int> >& cs = chains();
  for (int i = 0;i < CHAIN_NUMBER;++i)
  {
    const QVector<int>& originalChain = cs.at(i);
    //QVector<int> rotateChain = doubleChain(originalChain);
    int needRotateCount = 0;
    int currentFillPos = originalChain.size() - 1;
    for (int j = currentFillPos;j >= 0;--j)
    {
      PixmapItem *ball = balls[originalChain.at(j)];
      if (!ball) // 此格真没球
      {
        ++needRotateCount;
        continue;
      }
//      if (ball->state() == PixmapItem::JUST_CREATED) // 此格还没填上来
//        ++needRotateCount;
      switch (ball->state())
      {
      case PixmapItem::USER_MOVING:
        // TODO 打断用户的操作
      case PixmapItem::STABLE: // 有稳定的球
      case PixmapItem::SYSTEM_MOVING: // 有正在转的球
        if (needRotateCount != 0)
        {
          int current = originalChain.at(j);
          int target = originalChain.at(j + needRotateCount);
          ball->rotateWithChain(originalChain,
                                current,
                                target,
                                PixmapItem::SYSTEM_MOVING, qMax(5, 20 - ball->leftAnimSteps()),
                                true);
          balls[target] = balls[current];
          balls[current] = NULL;
        }
        break;
      case PixmapItem::JUST_CREATED:
        int current = originalChain.at(0);
        int target = originalChain.at(j + needRotateCount);
        ball->rotateWithChain(originalChain,
                              current,
                              target,
                              PixmapItem::SYSTEM_MOVING, qMax(5, 20 - ball->leftAnimSteps()),
                              true);

      }
    }
  }
}
