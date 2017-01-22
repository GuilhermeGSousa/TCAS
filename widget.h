#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemAnimation>
#include "scenemanager.h"
#include "pointer.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void setup();
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);


private:
    Ui::Widget *ui;
    double scale;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    int totalWidth;
    int totalHeight;
    QPixmap plane;
    QPixmap intruder;
    QGraphicsPixmapItem* intr;
    QGraphicsPixmapItem* pln;
    qint32 radius;
    QTimer *time;
    SceneManager *scene_manager;
    Broadcaster *broadcaster;



};

#endif // WIDGET_H
