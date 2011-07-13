// Some defination of the game, and here are quite a lot of funtions to
// do the location

#ifndef GAMELOCATION_H
#define GAMELOCATION_H

#include <QPoint>
#include <QVector>

// PI
#define PI                                      3.14159265358979

// Refresh times per second
#define REFRESH_PER_SEC                         20

// Rad to angle
inline qreal radiansToAngle(qreal r)
{ return r * 180 / PI;}

// Angle to rad
inline qreal angleToRadians(qreal a)
{ return a / 180 * PI;}

// Returns the angle of the point acording to the origin
// Result should be in [0, 2*PI)
qreal angle(QPointF point, QPointF origin);

// Calculate the position acording to the angle, radius and origin
QPointF calculatePosition(qreal a, qreal r, QPointF origin);

// When the translate path is ^, suppose that the distance between begin
// position and end position is 1, inputs the x position, returns the y
// position(I should draw a picture to show how it works-.-)
qreal bridgeY(qreal bridgeX);

// The width of the item
#define ITEM_WIDTH                              20

// The height of the item
#define ITEM_HEIGHT                             20

// The absolute position of game(I must draw a picture~~~)
#define LOCATION_GAME_VIEW_X_FROM               0
#define LOCATION_GAME_VIEW_Y_FROM               0
#define LOCATION_GAME_VIEW_X_TO                 800
#define LOCATION_GAME_VIEW_Y_TO                 500

#define LOCATION_GAME_BOARD_X_FROM              (LOCATION_GAME_VIEW_X_FROM + 200) // 200
#define LOCATION_GAME_BOARD_Y_FROM              20
#define LOCATION_GAME_BOARD_X_TO                (LOCATION_GAME_VIEW_X_TO - 20)    // 780
#define LOCATION_GAME_BOARD_Y_TO                (LOCATION_GAME_VIEW_Y_TO - 20)    // 480

#define LOCATION_GAME_BOARD_ITEM_X_FROM         (LOCATION_GAME_BOARD_X_FROM + ITEM_WIDTH)  // 220
#define LOCATION_GAME_BOARD_ITEM_Y_FROM         (LOCATION_GAME_BOARD_Y_FROM + ITEM_WIDTH)  // 40
#define LOCATION_GAME_BOARD_ITEM_X_TO           (LOCATION_GAME_BOARD_X_TO - 2 * ITEM_WIDTH) // 740
#define LOCATION_GAME_BOARD_ITEM_Y_TO           (LOCATION_GAME_BOARD_Y_TO - 2 * ITEM_HEIGHT) // 420

#define LOCATION_GAME_BOARD_ITEM_X_INTERVAL     (LOCATION_GAME_BOARD_ITEM_X_TO - \
                                                 LOCATION_GAME_BOARD_ITEM_X_FROM) / \
                                                 COLUMN_NUMBER
#define LOCATION_GAME_BOARD_ITEM_Y_INTERVAL     (LOCATION_GAME_BOARD_ITEM_Y_TO - \
                                                 LOCATION_GAME_BOARD_ITEM_Y_FROM) / \
                                                 ROW_NUMBER

// Total item number
#define TOTAL_ITEM_NUMBER                       61
// Total row number
#define ROW_NUMBER                              9
// Total column number
#define COLUMN_NUMBER                           18
// Total chain(circle) number
#define CHAIN_NUMBER                            5

// The offset of the scene and mouse
#define SCENE_TO_MOUSE_DX                       (ITEM_WIDTH / 2)
#define SCENE_TO_MOUSE_DY                       (ITEM_HEIGHT / 2)

// The gestures
enum GESTURE {SWAP, SLIDE, ROTATE};

// The gesture directions(currently, it's used only for the slide gesture)
enum GESTURE_DIRECTION {NO_DIRECTION,
                        LEFT_RIGHT,
                        LEFT_UP_RIGHT_DOWN,
                        LEFT_DOWN_RIGHT_UP};

// The state of the gesture
enum GESTURE_STATE {NO_GESTURE, CHOOSE_GESTURE, LOCATE_GESTURE, BAD_GESTURE}; // TODO:BAD_GESTURE要整合进去

// Distance of two points
qreal distanceOfTwoPoints(QPointF p1, QPointF p2);

// The column number of the item with the index
int columnOfIndex(int index);

// The row number of the item with the index
int rowOfIndex(int index);

// Left Top of the item with the index
QPointF positionOfIndex(int index);

// The chains to reload items(from inner to outter(我这个是不是拼错了-.-))
const QVector<QVector<int> >& chains();

// The chain of the item with the index to reload items
QVector<int> chainOfIndex(int index, bool full = false);

// The chain of the item with the index to rotate items
QVector<int> chainOfIndex(int from,
                          int to,
                          bool counterclockwise = false);

// Index of the near by position in 6 directions
int leftIndex(int index);
int leftUpIndex(int index);
int rightUpIndex(int index);
int rightIndex(int index);
int rightDownIndex(int index);
int leftDownIndex(int index);

// Double the chain to meet the demands of a function of pixmap
QVector<int> doubleChain(const QVector<int>& v);

// ABANDONED!!!
// Abandoned because I found that if I use this one to rotate, I have to call
// this function 6 times and do lots of judgement, finally I find a perfect way
// to solve the probleam
// Indexes around the index center
// Start with from, end with to
// if counterclockwise then force to be counterclockwise
// else select the nearest way
//QVector<int> chainAroundIndex(int from,
//                              int to,
//                              int center,
//                              bool counterclockwise = false);

// Indexes around the index
// At most 6 indexes
// The order is left, leftUp, rightUp, right, rightDown, leftDown
QVector<int> chainAroundIndex(int index);

// Whether the index can be a center to rotate
bool canBeRotateCenter(int index);

// Whether the index is a joint of the chains(circles)
bool isJoint(int index);

// The index of the item at the position of the scene
int indexOfPosition(QPointF position);

// The index of the item at the position of the row and column
int indexOfPosition(int row, int column);

// The index of the item at the position of the mouse
int indexOfMousePosition(QPointF position);

// Index of the first/last position in 6 directions
int firstOfRow(int row);
int lastOfRow(int row);
int firstOfLeftUp(int index);
int lastOfLeftUp(int index);
int firstOfLeftDown(int index);
int lastOfLeftDown(int index);

// Distance from the center with the angle, helps to calculate the distance
// from the center and locate the balls on the hexagon
qreal distanceFromTheCenterWithTheAngle(qreal angle);

#endif // GAMELOCATION_H
