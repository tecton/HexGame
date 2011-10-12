#include "twoplayertiminggamewidget.h"

#include <QPainter>
#include <QTimer>
#include "timinggamewidget.h"
#include "pixmapoperations.h"

#define LOGICAL_WIDTH    1024
#define LOGICAL_HEIGHT   600

#define GAME1_X_FROM     108
#define GAME1_X_TO       458
#define GAME2_X_FROM     566
#define GAME2_X_TO       916

#define GAME1_WIDTH      LOGICAL_HEIGHT
#define GAME1_HEIGHT     (GAME1_X_TO - GAME1_X_FROM)
#define GAME2_WIDTH      LOGICAL_HEIGHT
#define GAME2_HEIGHT     (GAME2_X_TO - GAME2_X_FROM)

#define END_ENIM_LAST    40
#define BEGIN_ENIM_LAST  40

TwoPlayerTimingGameWidget::TwoPlayerTimingGameWidget
    (AbstractRule::Gesture gesture) :
    game1(NULL),
    game2(NULL),
    result1(-1),
    result2(-1),
    beginAnim(BEGIN_ENIM_LAST),
    endAnim(-1),
    ges(gesture)
{
  // Create the timers and connect signals and slots
  t = new QTimer();
  t->setInterval(75);
  connect(t, SIGNAL(timeout()), this, SLOT(advance()));
}

TwoPlayerTimingGameWidget::~TwoPlayerTimingGameWidget()
{
  t->stop();
  delete t;
  if (game1)
  {
    delete game1;
    game1 = NULL;
  }
  if (game2)
  {
    delete game2;
    game2 = NULL;
  }
}

void TwoPlayerTimingGameWidget::makePixmap(
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

void TwoPlayerTimingGameWidget::makeBasicPixmap(
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
  if (beginAnim > 0)
  {
  }
  else if (endAnim > 0)
  {
  }
  else
  {
    painter->translate(GAME1_X_TO, 0);
    painter->rotate(90);
    if (game1)
    {
#ifdef USE_PIXMAP
      QPixmap tmp;
      game1->makePixmap(tmp, GAME1_WIDTH, GAME1_HEIGHT);
      painter->drawPixmap(0, 0, tmp);
#else
      game1->makePixmap(painter, GAME1_WIDTH, GAME1_HEIGHT);
#endif
    }
    else
    {
    }
    painter->rotate(-90);
    painter->translate(-GAME1_X_TO, 0);

    painter->translate(GAME2_X_FROM, LOGICAL_HEIGHT);
    painter->rotate(-90);

    if (game2)
    {
#ifdef USE_PIXMAP
      QPixmap tmp;
      game2->makePixmap(tmp, GAME2_WIDTH, GAME2_HEIGHT);
      painter->drawPixmap(0, 0, tmp);
#else
      game2->makePixmap(painter, GAME2_WIDTH, GAME2_HEIGHT);
#endif
    }
    else
    {
    }

    painter->rotate(90);
    painter->translate(-GAME2_X_FROM, -LOGICAL_HEIGHT);
  }

#ifdef USE_PIXMAP
  // End the paint and release the space
  painter->end();
  delete painter;
#endif
}

void TwoPlayerTimingGameWidget::addEffect(
#ifdef USE_PIXMAP
    QPixmap& pixmap,
#else
    QPainter* painter,
#endif
                                 int width,
                                 int height)
{}

QPointF TwoPlayerTimingGameWidget::toScene(double xRate, double yRate)
{
  return QPointF(xRate * LOGICAL_WIDTH,
                 yRate * LOGICAL_HEIGHT);
}

int whichGame(QPointF mousePos)
{
  double x = mousePos.x();
  if (x >= GAME1_X_FROM && x <= GAME1_X_TO)
    return 1;
  if (x >= GAME2_X_FROM && x <= GAME2_X_TO)
    return 2;
}

QPointF TwoPlayerTimingGameWidget::gamePos(QPointF mousePos, int gameIndex)
{
  switch (gameIndex)
  {
  case 1:
    return game1->toScene(mousePos.y() / LOGICAL_HEIGHT,
                          (GAME1_X_TO - mousePos.x()) / GAME1_HEIGHT);
    break;
  case 2:
    return game2->toScene(1 - mousePos.y() / LOGICAL_HEIGHT,
                          (mousePos.x() - GAME2_X_FROM) / GAME2_HEIGHT);
    break;
  default:
    return mousePos;
    break;
  }
}

void TwoPlayerTimingGameWidget::dealPressed(QPointF mousePos,
                         Qt::MouseButton button)
{
  switch (whichGame(mousePos))
  {
  case 1:
    if (game1)
      game1->dealPressed(gamePos(mousePos, 1), button);
    break;
  case 2:
    if (game2)
      game2->dealPressed(gamePos(mousePos, 2), button);
    break;
  default:
    break;
  }
}

void TwoPlayerTimingGameWidget::dealMoved(QPointF mousePos,
                       Qt::MouseButton button)
{
  switch (whichGame(mousePos))
  {
  case 1:
    if (game1)
      game1->dealMoved(gamePos(mousePos, 1), button);
    break;
  case 2:
    if (game2)
      game2->dealMoved(gamePos(mousePos, 2), button);
    break;
  default:
    break;
  }
}

void TwoPlayerTimingGameWidget::dealReleased(QPointF mousePos,
                          Qt::MouseButton button)
{
  switch (whichGame(mousePos))
  {
  case 1:
    if (game1)
      game1->dealReleased(gamePos(mousePos, 1), button);
    break;
  case 2:
    if (game2)
      game2->dealReleased(gamePos(mousePos, 2), button);
    break;
  default:
    break;
  }
}

void TwoPlayerTimingGameWidget::getForcus()
{
  t->start();
}

void TwoPlayerTimingGameWidget::start()
{
  game1 = new TimingGameWidget(ges);
  game2 = new TimingGameWidget(ges);
  connect(game1,
          SIGNAL(totalScore(TimingGameWidget*,int)),
          this,
          SLOT(totalScore(TimingGameWidget*,int)));
  connect(game2,
          SIGNAL(totalScore(TimingGameWidget*,int)),
          this,
          SLOT(totalScore(TimingGameWidget*,int)));
  endAnim = -1;
  result1 = 0;
  result2 = 0;
  game1->getForcus();
  game2->getForcus();
}

// Advance
void TwoPlayerTimingGameWidget::advance()
{
  if (beginAnim > 0)
  {
    --beginAnim;
    if (beginAnim == 0)
      start();
  }
  else if (endAnim > 0)
  {
    --endAnim;
    if (beginAnim == 0)
    {
      emit giveControlTo(NULL, true);
      delete this;
      return;
    }
  }
}

// A game will emit this thing to show its end with a score
void TwoPlayerTimingGameWidget::totalScore(TimingGameWidget *whoAmI,
                                           int score)
{
  if (whoAmI == game1)
  {
    result1 = score;
    game1 = NULL;
  }
  else
  {
    result2 = score;
    game2 = NULL;
  }
  if ((!game1) && (!game2))
  {
    endAnim = END_ENIM_LAST;

  }
}
