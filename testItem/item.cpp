#include "item.h"

#include <QPainter>
#include <QColor>
#include <QLinearGradient>
#include <QFont>
#include <QPolygonF>
#include <QFontMetrics>

#define LOGICAL_WINDOW_WIDTH           300
#define LOGICAL_WINDOW_HEIGHT          200
#define LOGICAL_ITEM_R                 80.0
#define LOGICAL_DIGIT_SIZE             80
#define LOGICAL_ADVANCE_SIZE           18

#define ONE_ROUND_STEPS                30
#define FONT                           "Arial Rounded MT Bold"

#define DIGIT_NUMBER                   5 // should be "stage", here is only a test

QColor backgroundDarkColors[]  = {QColor(180,   0,   0, 255)};
QColor backgroundLightColors[] = {QColor(220,  50,  50, 255)};
QColor borderDarkColors[]      = {QColor(140, 140, 140, 255)};
QColor borderLightColors[]     = {QColor(180, 180, 180, 255)};
QColor digitBackgroundColors[] = {QColor(255, 200, 200, 150)};
QColor digitColors[]           = {QColor(200,  30,  30, 200)};
QColor advanceColors[]         = {QColor(250, 150, 150, 150)};

// Draw the text at the given place
// The point (x,y) is the center of the text
void drawTextAt(double x, double y, QPainter *painter, QString text, double rotate = 0)
{
  painter->translate(x, y);
  painter->rotate(rotate);
  QFontMetrics fm = painter->fontMetrics();
  QPoint pt(0 - fm.width(text) / 2,
            0 - (fm.ascent() + fm.descent()) / 2 + fm.ascent());
  painter->drawText(pt,text);
  painter->rotate(-rotate);
  painter->translate(-x, -y);
}

Item::Item(int s, QPointF pos, bool l, bool a) :
    stage(s),
    position(pos),
    advance(a),
    locked(l)
{
}

void Item::paint(QPainter *painter, int width, int height, int frame)
{


  double fontScale = qMin(1.0 * width / LOGICAL_WINDOW_WIDTH,
                          1.0 * height / LOGICAL_WINDOW_HEIGHT);

  QPointF centerPos = QPointF(position.x() * width, position.y() * height);

  QPointF gFrom = centerPos;
  gFrom.setX(gFrom.x() + (LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH) *
             (frame - ONE_ROUND_STEPS) / ONE_ROUND_STEPS);

  QPointF gTo = centerPos;
  gTo.setX(gTo.x() - (LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH));
  gTo.setY(gTo.y() + 0.577 * (LOGICAL_ITEM_R * height / LOGICAL_WINDOW_HEIGHT));
  gTo.setX(gTo.x() + (LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH) *
           (frame - ONE_ROUND_STEPS / 2) / ONE_ROUND_STEPS);

  QLinearGradient gradient = QLinearGradient(gFrom, gTo);
  gradient.setColorAt(0, backgroundDarkColors[stage - 1]);
  gradient.setColorAt(1, backgroundLightColors[stage - 1]);
  gradient.setSpread(QGradient::ReflectSpread);

  painter->setBrush(QBrush(gradient));

  gradient = QLinearGradient(gFrom, gTo);
  gradient.setColorAt(0, borderDarkColors[stage - 1]);
  gradient.setColorAt(1, borderLightColors[stage - 1]);
  gradient.setSpread(QGradient::ReflectSpread);
  painter->setPen(QPen(QBrush(gradient), 4 * fontScale));

  // Calculate the polygon to paint
  QVector<QPointF> points;
  points.push_back(QPointF(centerPos.x() - LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH,
                           centerPos.y()));
  points.push_back(QPointF(centerPos.x() - 0.5 * LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH,
                           centerPos.y() - 0.866 * LOGICAL_ITEM_R * height / LOGICAL_WINDOW_HEIGHT));
  points.push_back(QPointF(centerPos.x() + 0.5 * LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH,
                           centerPos.y() - 0.866 * LOGICAL_ITEM_R * height / LOGICAL_WINDOW_HEIGHT));
  points.push_back(QPointF(centerPos.x() + LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH,
                           centerPos.y()));
  points.push_back(QPointF(centerPos.x() + 0.5 * LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH,
                           centerPos.y() + 0.866 * LOGICAL_ITEM_R * height / LOGICAL_WINDOW_HEIGHT));
  points.push_back(QPointF(centerPos.x() - 0.5 * LOGICAL_ITEM_R * width / LOGICAL_WINDOW_WIDTH,
                           centerPos.y() + 0.866 * LOGICAL_ITEM_R * height / LOGICAL_WINDOW_HEIGHT));
  QPolygonF poly(points);

  painter->drawPolygon(poly);

  QFont font;
  font.setFamily(FONT);
  font.setBold(true);
  font.setPointSize(LOGICAL_DIGIT_SIZE * 1.05 * fontScale);
  painter->setFont(font);

  painter->setPen(digitBackgroundColors[stage - 1]);
  drawTextAt(centerPos.x(), centerPos.y(), painter, QObject::tr("%1").arg(DIGIT_NUMBER));

  font.setBold(false);
  font.setPointSize(LOGICAL_DIGIT_SIZE * fontScale);
  painter->setFont(font);

  painter->setPen(QColor(digitColors[stage - 1]));
  drawTextAt(centerPos.x(), centerPos.y(), painter, QObject::tr("%1").arg(DIGIT_NUMBER));

  QFont font2;
  font2.setFamily(FONT);
  font2.setBold(true);
  font2.setPointSize(LOGICAL_ADVANCE_SIZE * fontScale);
  painter->setFont(font2);

  painter->setPen(QColor(advanceColors[stage - 1]));
  drawTextAt(centerPos.x(), centerPos.y(), painter, "Advanced", -30);
}
