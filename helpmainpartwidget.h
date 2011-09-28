#ifndef HELPMAINPARTWIDGET_H
#define HELPMAINPARTWIDGET_H

// File must include
#include <QPixmap>
#include <QPointF>
#include <QString>
#include <QVector>

class QPainter;

/**
 * @brief A class to show the main part of the help.
 */
class HelpMainPartWidget
{
public:
  /**
   *@brief Constructor.
   *
   *@param background Path The path of the background of the background.
   *@param animCenters The centers of the parts with animation.
   *@param animPaths The paths of the parts with animation.
   */
  HelpMainPartWidget(QString backgroundPath,
                     QVector<QPointF> animCenters,
                     QVector<QString> animPaths);


  /**
   *@brief Paint the main part.
   *
   *@param painter The painter.
   *@param width The width.
   *@param height The height.
   *@param frame The index of the frame to show.
   */
  void paint(QPainter *painter, int width, int height, int frame);

private:
  int backgroundFrameCounts;
  QVector<QPixmap> backgroundPixmaps;
  QVector<QPointF> animCenterPoints;
  QVector<int> animFrameCounts;
  QVector<QVector<QPixmap> > animPixmaps;
};

#endif // HELPMAINPARTWIDGET_H
