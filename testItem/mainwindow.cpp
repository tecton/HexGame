#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "item.h"
#include <QPainter>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    frame(0)
{
    ui->setupUi(this);
    item = new Item(1, QPointF(0.5, 0.5), false, false);
    t = new QTimer();

    t->setInterval(30);
    connect(t, SIGNAL(timeout()), this, SLOT(advance()));
    t->start();
}

MainWindow::~MainWindow()
{
  t->stop();
  delete t;
  delete item;
  delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
  QPainter *painter = new QPainter(this);

  item->paint(painter, width(), height(), frame);

  painter->end();
  delete painter;
}

void MainWindow::advance()
{
  ++frame;
  update();
}
