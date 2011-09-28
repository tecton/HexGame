#ifndef HELPITEM_H
#define HELPITEM_H

// File must include
#include "abstractitem.h"
#include <QVector>
#include <QPixmap>
#include <QString>

/**
 * @brief A class of an item of a button in help widget.
 */
class HelpItem : public AbstractRectItem
{
public:
  /**
   * @brief Total items to show the help.
   */
  static int totalItems();

  /**
   * @brief Constructor.
   */
  HelpItem(int theIndex);

  virtual void paint(QPainter *painter, int width, int height, int frame);

  virtual double width();
  virtual double height();

  /**
   * @brief Get the path of the background of the main part.
   */
  QString mainPartBackGroundPath();

  /**
   * @brief Get the centers of the parts with animation.
   */
  QVector<QPointF> mainPartAnimCenters();

  /**
   * @brief Get the paths of the parts with animation.
   */
  QVector<QString> mainPartAnimPaths();

private:
  int index;
};

#endif // HELPITEM_H
