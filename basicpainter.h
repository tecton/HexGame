/*
** A class to do the basic paint.
** Currently, the functions are all static, because I haven't found anything to
** store in the painter.
*/

#ifndef BASICPAINTER_H
#define BASICPAINTER_H

// File must include
#include <QVector>

// Forward declaration
class Ball;
class AbstractGameBoardInfo;
class AbstractItem;
class QPainter;

class BasicPainter
{
public:
  // The id of the background
  enum BackgroundId {MainMenu=0,
                     PuzzleMenu,
                     StageMenu,
                     Game37,
                     Game61,
                     PuzzleGame=Game61};

  // A function to paint the background
  static void paintBackGround(BackgroundId id,
                              QPainter *painter,
                              int width,
                              int height,
                              int frame);

  // A function to paint the balls
  static void paintBasicBalls(AbstractGameBoardInfo *
                                  gameboard,
                              Ball **balls,
                              int totalCount,
                              QPainter *painter,
                              double xRate,
                              double yRate,
                              int frame);

  // A function to paint small balls for puzzle game target
  // shape
  static void paintPuzzleGameBalls(AbstractGameBoardInfo *
                                       gameboard,
                                   int *colorIndex,
                                   int totalCount,
                                   QPainter *painter,
                                   double xRate,
                                   double yRate,
                                   int frame,
                                   int size);

  // A function to paint items inherit from AbstractItem
  static void paintItems(QPainter *painter,
                         QVector<AbstractItem *> items,
                         int width,
                         int height,
                         int frame);

  // A function to darken the things which has benn painted
  static void darken(QPainter *painter, int width, int height);
};

#endif // BASICPAINTER_H
