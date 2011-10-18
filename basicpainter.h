#ifndef BASICPAINTER_H
#define BASICPAINTER_H

// File must include
#include <QVector>
#include <QPointF>

// Forward declaration
class Ball;
class AbstractGameBoardInfo;
class AbstractItem;
class QPainter;

/**
 * @brief A class to do the basic paint.
 *
 * Currently, the functions are all static, because I haven't found anything to
 * store in the painter.
 */
class BasicPainter
{
public:
  /**
   * @brief The id of the backgrounds.
   */
  enum BackgroundId {MainMenu=0,
                     PuzzleMenu,
                     StageMenu,
                     Game37,
                     Game61,
                     PuzzleGame=Game61,
                     Help=5,
                     Achievement};

  /**
   *@brief Paint the background.
   *
   *@param id The id of the background.
   *@param painter The painter.
   *@param width The width.
   *@param height The height.
   *@param frame The index of the frame to show.
   */
  static void paintBackGround(BackgroundId id,
                              QPainter *painter,
                              int width,
                              int height,
                              int frame);

  /**
   *@brief Paint the basic balls.
   *
   *@param gameboard The gameboard.
   *@param balls The balls.
   *@param totalCount Count of the balls.
   *@param painter The painter.
   *@param xRate The scale in X direction.
   *@param yRate The scale in Y direction.
   *@param frame The index of the frame to show.
   */
  static void paintBasicBalls(AbstractGameBoardInfo *gameboard,
                              Ball **balls,
                              int totalCount,
                              QPainter *painter,
                              double xRate,
                              double yRate,
                              int frame);
  /**
   *@brief Paint the basic balls.
   *
   *@param gameboard The gameboard.
   *@param balls The balls.
   *@param totalCount Count of the balls.
   *@param painter The painter.
   *@param xRate The scale in X direction.
   *@param yRate The scale in Y direction.
   *@param frame The index of the frame to show.
   */
  static void paintBasicBalls(Ball **balls,
                              int totalCount,
                              QPainter *painter,
                              double xRate,
                              double yRate,
                              int frame,
                              QPointF *positions,
                              bool clockwise);

  /**
   *@brief Paint the small balls in the puzzle mode.
   *
   *@param gameboard The gameboard.
   *@param colorIndex The index of the color of the balls.
   *@param totalCount Count of the balls.
   *@param painter The painter.
   *@param xRate The scale in X direction.
   *@param yRate The scale in Y direction.
   *@param frame The index of the frame to show.
   *@param size The size of the balls to show, should be 1 to 6.
   */
  static void paintPuzzleGameBalls(AbstractGameBoardInfo *gameboard,
                                   int *colorIndex,
                                   int totalCount,
                                   QPainter *painter,
                                   double xRate,
                                   double yRate,
                                   int frame,
                                   int size);

  /**
   *@brief Paint items.
   *
   *@param painter The painter.
   *@param items The items.
   *@param width The width of the whole QPaintDevice.
   *@param height The height of the whole QPaintDevice.
   *@param frame The index of the frame to show.
   */
  static void paintItems(QPainter *painter,
                         QVector<AbstractItem *> items,
                         int width,
                         int height,
                         int frame);

  /**
   *@brief Darken the things which has been painted.
   *
   * In fact it only draws a rectangle to cover the original image.
   *@param painter The painter.
   *@param width The width of the whole QPaintDevice.
   *@param height The height of the whole QPaintDevice.
   */
  static void darken(QPainter *painter, int width, int height);
};

#endif // BASICPAINTER_H
