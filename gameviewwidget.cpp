#include <QGridLayout>
#include <QMessageBox>
#include <QBrush>
#include "gameviewwidget.h"
#include "gamelocation.h"

// 测试此手势：
//   正确手势：
//     将_gesture_state从CHOOSE_GESTURE改为LOCATE_GESTURE
//     将_gesture,
//       _gesture_direction,
//       _gesture_influenced_indexes,
//       _gesture_influenced_indexes_original_pos改为正确值
//     需要时将_gesture_rotate_center_pos改为正确值

bool GameViewWidget::testGesture()
{
  if (_gesture_state == CHOOSE_GESTURE)
  {
    if (_gesture_indexes.size() >= 3)
    {
      { // 左右
        int r0 = rowOfIndex(_gesture_indexes[0]);
        int r1 = rowOfIndex(_gesture_indexes[1]);
        int r2 = rowOfIndex(_gesture_indexes[2]);
        if (r0 == r1 && r1 == r2)
        {
          _gesture_state = LOCATE_GESTURE;
          _gesture = SLIDE;
          _gesture_direction = LEFT_RIGHT;

          int current = firstOfRow(r0);
          _gesture_influenced_indexes.clear();
          _gesture_influenced_indexes_original_pos.clear();
          while (current != -1)
          {
            _gesture_influenced_indexes.push_back(current);
            _gesture_influenced_indexes_original_pos.push_back(positionOfIndex(current));
            current = rightIndex(current);
          }
          return true;
        }
      }
      {  // 左上右下
        int lu0 = firstOfLeftUp(_gesture_indexes[0]);
        int lu1 = firstOfLeftUp(_gesture_indexes[1]);
        int lu2 = firstOfLeftUp(_gesture_indexes[2]);
        if (lu0 == lu1 && lu1 == lu2)
        {
          _gesture_state = LOCATE_GESTURE;
          _gesture = SLIDE;
          _gesture_direction = LEFT_UP_RIGHT_DOWN;

          int current = lu0;
          _gesture_influenced_indexes.clear();
          _gesture_influenced_indexes_original_pos.clear();
          while (current != -1)
          {
            _gesture_influenced_indexes.push_back(current);
            _gesture_influenced_indexes_original_pos.push_back(positionOfIndex(current));
            current = rightDownIndex(current);
          }
          return true;
        }
      }
      {  // 左下右上
        int ld0 = firstOfLeftDown(_gesture_indexes[0]);
        int ld1 = firstOfLeftDown(_gesture_indexes[1]);
        int ld2 = firstOfLeftDown(_gesture_indexes[2]);
        if (ld0 == ld1 && ld1 == ld2)
        {
          _gesture_state = LOCATE_GESTURE;
          _gesture = SLIDE;
          _gesture_direction = LEFT_DOWN_RIGHT_UP;

          int current = ld0;
          _gesture_influenced_indexes.clear();
          _gesture_influenced_indexes_original_pos.clear();
          while (current != -1)
          {
            _gesture_influenced_indexes.push_back(current);
            _gesture_influenced_indexes_original_pos.push_back(positionOfIndex(current));
            current = rightUpIndex(current);
          }
          return true;
        }
      }
    }

  }
  return false;
}

QVector<QPointF> GameViewWidget::newposUnderPos(QPointF mousePos)
{
  QVector<QPointF> result = _gesture_influenced_indexes_original_pos;
  if (_gesture_state != LOCATE_GESTURE)
    return result;
  if (_gesture == SLIDE)
  {
    int firstIndex = _gesture_influenced_indexes[0];
    int lastIndex = _gesture_influenced_indexes[_gesture_influenced_indexes.size() - 1];
    QPointF firstPos = positionOfIndex(firstIndex);
    QPointF lastPos = positionOfIndex(lastIndex);
    int minX = firstPos.x();
    int minY = firstPos.y();
    int maxX = lastPos.x();
    int maxY = lastPos.y();

    qreal dx = 0;
    qreal dy = 0;
    qreal mouseMovedX = mousePos.x() - _gesture_confirm_pos.x();
    qreal yRate = 0;
    if (_gesture_direction == LEFT_RIGHT)
    {
      dx = LOCATION_GAME_BOARD_ITEM_X_INTERVAL;
      dy = 0;
    }
    else if (_gesture_direction == LEFT_UP_RIGHT_DOWN)
    {
      dx = LOCATION_GAME_BOARD_ITEM_X_INTERVAL / 2;
      dy = LOCATION_GAME_BOARD_ITEM_Y_INTERVAL / 2;
    }
    else if (_gesture_direction == LEFT_DOWN_RIGHT_UP)
    {
      dx = LOCATION_GAME_BOARD_ITEM_X_INTERVAL / 2;
      dy = - (LOCATION_GAME_BOARD_ITEM_Y_INTERVAL / 2);
    }
    yRate = dy / dx;
    minX -= dx;
    minY -= dy;
    maxX += dx;
    maxY += dy;
    dx = maxX - minX;
    dy = maxY - minY;

    for (int i = 0;i < _gesture_influenced_indexes_original_pos.size();++i)
    {
      QPointF original = result[i];
      QPointF current = original;
      current.setX(original.x() + mouseMovedX);
      current.setY(original.y() + mouseMovedX * yRate);
      while (current.x() < minX)
      {
        current.setX(current.x() + (dx));
        current.setY(current.y() + (dy));
      }
      while (current.x() > maxX)
      {
        current.setX(current.x() - (dx));
        current.setY(current.y() - (dy));
      }
      result[i] = current;
      qreal movedX = current.x() - original.x();
      int movedColumns = movedX * COLUMN_NUMBER / dx;
      int movedRows = 0;
      if (_gesture_direction == LEFT_RIGHT)
        movedRows = 0;
      else if (_gesture_direction == LEFT_UP_RIGHT_DOWN)
        movedRows = movedColumns/2;
      else if (_gesture_direction == LEFT_DOWN_RIGHT_UP)
        movedRows = -movedColumns/2;
      int c = columnOfIndex(_gesture_influenced_indexes[i]);
      int r = rowOfIndex(_gesture_influenced_indexes[i]);
      c += movedColumns;
      r += movedRows;
      int index = indexOfPosition(r, c);
      ballsOriginalIndexToCurrentIndex[_gesture_influenced_indexes[i]] = index;
      ballsCurrentIndexToOriginalIndex[index] = _gesture_influenced_indexes[i];
    }
  }
  return result;
}

GameViewWidget::GameViewWidget(QWidget *parent) :
    QWidget(parent),
    _gesture_state(NO_GESTURE)
{
  scene = new QGraphicsScene();
  scene->setSceneRect(LOCATION_GAME_VIEW_X_FROM,
                      LOCATION_GAME_VIEW_Y_FROM,
                      LOCATION_GAME_VIEW_X_TO,
                      LOCATION_GAME_VIEW_Y_TO);
  view = new GameGraphicsView(scene);

  view->setFocusPolicy( Qt::StrongFocus );
  view->setBackgroundBrush(QBrush(QColor(0,0,0)));
  QGridLayout *layout = new QGridLayout();
  layout->addWidget(view);
  setLayout(layout);

  balls = new PixmapItem*[TOTAL_ITEM_NUMBER];
  for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
  {
    balls[i] = NULL;
    ballsCurrentIndexToOriginalIndex[i] = -1;
    ballsOriginalIndexToCurrentIndex[i] = -1;
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


  connect(view,
          SIGNAL(mouseMoved(QMouseEvent*)),
          this,
          SLOT(dealMoved(QMouseEvent*)));
  connect(view,
          SIGNAL(mousePressed(QMouseEvent*)),
          this,
          SLOT(dealPressed(QMouseEvent*)));
  connect(view,
          SIGNAL(mouseReleased(QMouseEvent*)),
          this,
          SLOT(dealReleased(QMouseEvent*)));

  t = new QTimer();
  t->setInterval(1000/REFRESH_PER_SEC);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
  t->start();

}

void GameViewWidget::dealMoved(QMouseEvent *event)
{
//  QMessageBox::critical(0,"","");
  if ((event->buttons() & Qt::LeftButton) != Qt::LeftButton)
    return;
  QPointF p = view->mapToScene(event->pos());
  if (_gesture_state == CHOOSE_GESTURE)
  {
    int index = indexOfPosition(p);
    if (index >= 0)
    {
//      _gesture_indexes.clear();
      if ((_gesture_indexes.size() > 0 &&
           _gesture_indexes[_gesture_indexes.size() - 1] != index) ||
          _gesture_indexes.size() == 0)
        _gesture_indexes.push_back(index);
    }
    if (testGesture())
      _gesture_confirm_pos = p;
  }
  if (_gesture_state == LOCATE_GESTURE)
  {
    //int
    if (_gesture == SLIDE)
    {
      QVector<QPointF> newPos = newposUnderPos(p);
      for (int i = 0;i < newPos.size();++i)
      {
        balls[_gesture_influenced_indexes[i]]->setPos(newPos[i]);

      }
    }
  }
}

void GameViewWidget::dealPressed(QMouseEvent *event)
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

//  if (event->button() == Qt::LeftButton)
//  {
//    QPointF p = view->mapToScene(event->pos());
////    QMessageBox::critical(0,"",QString::number(event->pos().x()) +
////                               "\n" +
////                               QString::number(event->pos().y()));
////    QMessageBox::critical(0,"",QString::number(p.x()) +
////                               "\n" +
////                               QString::number(p.y()));
//    int index = indexOfPosition(p);
////    QMessageBox::critical(0,"",QString::number(index));
//    if (index < 0 || index >= TOTAL_ITEM_NUMBER)
//      return;
//    if (balls[index])
//    {
//      balls[index]->recycle();
//      balls[index] = NULL;
//    }
//  }
//  if (event->button() == Qt::RightButton)
//  {
//    fillAllBlanks();
//  }
  _gesture_state = NO_GESTURE;
  QPointF p = view->mapToScene(event->pos());
  int index = indexOfPosition(p);
  if (index >= 0)
  {
    _gesture_state = CHOOSE_GESTURE;
    _gesture_indexes.clear();
    _gesture_indexes.push_back(index);
  }
}

void GameViewWidget::dealReleased(QMouseEvent *event)
{
//  QMessageBox::critical(0,"","");
  if (false) // 能消去
  {
    // 消去要做的事
  }
  else
  {
    if (_gesture_state == LOCATE_GESTURE)
    {
      if (_gesture == SLIDE)
      {
        for (int i = 0;i < _gesture_influenced_indexes.size();++i)
        {// 要优化！！！
          int currentIndex = _gesture_influenced_indexes[i];
          int jointIndex1 = 0;
          int jointIndex2 = 0;
          QPointF j1;
          QPointF j2;
          if (_gesture_direction == LEFT_UP_RIGHT_DOWN)
          {
            jointIndex1 = firstOfLeftUp(currentIndex);
            jointIndex2 = lastOfLeftUp(currentIndex);
          }
          else if (_gesture_direction == LEFT_DOWN_RIGHT_UP)
          {
            jointIndex1 = firstOfLeftDown(currentIndex);
            jointIndex2 = lastOfLeftDown(currentIndex);
          }
          else if (_gesture_direction == LEFT_RIGHT)
          {
            int r = rowOfIndex(currentIndex);
            jointIndex1 = firstOfRow(r);
            jointIndex2 = lastOfRow(r);
          }
          j1 = positionOfIndex(jointIndex1);
          j2 = positionOfIndex(jointIndex2);
          if (_gesture_direction == LEFT_UP_RIGHT_DOWN)
          {
            j1.setX(j1.x() - LOCATION_GAME_BOARD_ITEM_X_INTERVAL/2);
            j1.setY(j1.y() - LOCATION_GAME_BOARD_ITEM_Y_INTERVAL/2);
            j2.setX(j2.x() + LOCATION_GAME_BOARD_ITEM_X_INTERVAL/2);
            j2.setY(j2.y() + LOCATION_GAME_BOARD_ITEM_Y_INTERVAL/2);
          }
          else if (_gesture_direction == LEFT_DOWN_RIGHT_UP)
          {
            j1.setX(j1.x() - LOCATION_GAME_BOARD_ITEM_X_INTERVAL/2);
            j1.setY(j1.y() + LOCATION_GAME_BOARD_ITEM_Y_INTERVAL/2);
            j2.setX(j2.x() + LOCATION_GAME_BOARD_ITEM_X_INTERVAL/2);
            j2.setY(j2.y() - LOCATION_GAME_BOARD_ITEM_Y_INTERVAL/2);
          }
          else if (_gesture_direction == LEFT_RIGHT)
          {
            j1.setX(j1.x() - LOCATION_GAME_BOARD_ITEM_X_INTERVAL);
            j2.setX(j2.x() + LOCATION_GAME_BOARD_ITEM_X_INTERVAL);
          }

          balls[currentIndex]->slideBackTo(balls[currentIndex]->pos(),
                                           positionOfIndex(currentIndex),
                                           j1,
                                           j2,
                                           PixmapItem::SYSTEM_MOVING,
                                           10);

//          balls[_gesture_influenced_indexes[i]]->translateTo
//              (positionOfIndex(_gesture_influenced_indexes[i]),
//               PixmapItem::SYSTEM_MOVING,
//               10);
        }
        _gesture_state = NO_GESTURE;
        return;

      }
    }
    // 复位
    for (int i = 0;i < TOTAL_ITEM_NUMBER;++i)
    {
      ballsOriginalIndexToCurrentIndex[i] = i;
      ballsCurrentIndexToOriginalIndex[i] = i;
    }
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
