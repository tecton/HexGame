#ifndef MAINWIDGET_H
#define MAINWIDGET_H

// File must include
#include <QWidget>
#include <QVector>

// Forward declaration
class QTimer;
class QPixmap;
class AbstractPixmapWidget;
class SwapClassicGame;

/**
 * @brief A class which is the ONLY one which inherits QWidget in the project.
 *
 * It maintains a stack of pointers of widget in the game.
 */
class MainWidget : public QWidget
{
  Q_OBJECT
public:
  /**
   * @brief Constructor with the parent.
   */
  explicit MainWidget(QWidget *parent = 0);

  /**
   * @brief The paint event(may be called with a fixed interval).
   */
  void paintEvent(QPaintEvent *event);

protected:
  /**
   * @brief Mouse press event.
   */
  void mousePressEvent(QMouseEvent *event);

  /**
   * @brief Mouse move event.
   */
  void mouseMoveEvent(QMouseEvent *event);

  /**
   * @brief Mouse release event.
   */
  void mouseReleaseEvent(QMouseEvent *event);

private:
  // The timer to repaint
  QTimer *refreshTimer;

  // The current pixmap
  QPixmap pixmap;

  // The pixmap of the widget just lost control
  QPixmap lastPixmap;

  // The stack of the widgets
  QVector<AbstractPixmapWidget *> widgets;

  // The CD used when change the control
  int coolDown;

  // A function to calculate the position
  QPointF toScene(QPointF mousePosition);
signals:

public slots:
  /**
   * @brief A slot to change the control of the widgets in the stack.
   *
   * @param target The target to push into the stack(if not NULL).
   * @param deleteMySelf Whether to pop from the stack.
   */
  void changeControl(AbstractPixmapWidget *target, bool deleteMySelf);
};

#endif // MAINWIDGET_H
