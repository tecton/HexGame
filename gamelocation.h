#ifndef GAMELOCATION_H
#define GAMELOCATION_H

#include <QPoint>
#include <QVector>

#define PI                                      3.14159265358979
#define REFRESH_PER_SEC                         20

inline qreal radiansToAngle(qreal r)
{ return r * 180 / PI;}

inline qreal angleToRadians(qreal a)
{ return a / 180 * PI;}

qreal angle(QPointF point, QPointF origin);
QPointF calculatePosition(qreal a, qreal r, QPointF origin);

qreal bridgeY(qreal bridgeX);

#define LOCATION_GAME_VIEW_X_FROM               0
#define LOCATION_GAME_VIEW_Y_FROM               0
#define LOCATION_GAME_VIEW_X_TO                 800
#define LOCATION_GAME_VIEW_Y_TO                 500

#define LOCATION_GAME_BOARD_X_FROM              0
#define LOCATION_GAME_BOARD_Y_FROM              0
#define LOCATION_GAME_BOARD_X_TO                800
#define LOCATION_GAME_BOARD_Y_TO                500

#define ITEM_WIDTH                              20
#define ITEM_HEIGHT                             20

#define LOCATION_GAME_BOARD_ITEM_X_FROM         20
#define LOCATION_GAME_BOARD_ITEM_Y_FROM         20
#define LOCATION_GAME_BOARD_ITEM_X_TO           780 - ITEM_WIDTH
#define LOCATION_GAME_BOARD_ITEM_Y_TO           480 - ITEM_HEIGHT

#define TOTAL_ITEM_NUMBER                       61
#define ROW_NUMBER                              9
#define COLUMN_NUMBER                           18
#define CHAIN_NUMBER                            5

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

int leftUpIndex(int index);
int rightUpIndex(int index);
int rightIndex(int index);
int rightDownIndex(int index);
int leftDownIndex(int index);
int leftIndex(int index);
QVector<int> chainAroundIndex(int index);
QVector<int> doubleChain(const QVector<int>& v);
QVector<int> chainAroundIndex(int from,
                              int to,
                              int center,
                              bool counterclockwise = false);

// The index of the item at the position
int indexOfPosition(QPointF position);

bool isJoint(int index);

#endif // GAMELOCATION_H
