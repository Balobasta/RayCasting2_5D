#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (lastWidth != this->width()){
        source->changeNumOfRays(this->width());
        lastWidth = this->width();
        qDebug() << "changed";
        qDebug() << "NOR" << source->getNumOfRays();
    }
    QPainter painter(this);
    int n = source->getNumOfRays();
    int w = static_cast<int>(this->width()/ (n * 1.0) + 0.5);
    qDebug() << "w  " << w;
//    qDebug() << "n  " << n;
    QList<Scene> scene = source->getScene();
    for( int i = 0; i < n; i++){
        painter.fillRect(w*i, 0, w, this->height(),Qt::black);
        QColor grass(Qt::green);
        QColor sky(41, 187, 255);
        painter.fillRect(w*i, 0, w, this->height()/2, sky);
        painter.fillRect(w*i, this->height()/2, w, this->height()/2, grass);
        if(scene[i].dist == INFINITY) continue;
        int wallHeight = static_cast<int>(this->height()/ scene[i].dist);
        int bY = static_cast<int>((this->height()/2.0 - wallHeight/2.0));
        QColor c = scene[i].color;
        double k = 1/scene[i].dist;
        if ( k >= 0.9){
            k = 0.9;
        }
        c.setHslF(c.hslHueF(), c.hslSaturationF(), k);
        painter.fillRect(w*i, bY, w, wallHeight, c);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up){
        source->move(1);
    }
    if (event->key() == Qt::Key_Down){
        source->move(2);
    }
    if (event->key() == Qt::Key_Right){
        source->rotation(1);
    }
    if (event->key() == Qt::Key_Left){
        source->rotation(2);
    }
    repaint();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "X " << event->x() << "   Y " << event->y();
    qDebug() << "W " << this->width();
}
