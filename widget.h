#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsItemAnimation>
#include "scenemanager.h"
#include "sceneitems.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void on_leftButton_clicked();

    void on_rightButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_leftButton_pressed();

    void on_downButton_pressed();

    void on_rightButton_pressed();

    void on_upButton_pressed();

    void on_zoomIn_clicked();

    void on_zoomOut_clicked();

private:
    void setup();
    void setupListener(int portNum);

private:
    Ui::Widget *ui;
    double scale;
    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    int totalWidth;
    int totalHeight;
    qint32 radius;
    QTimer *time, *send_timer;
    SceneItems *scene_items;
    SceneManager *scene_manager;
    SceneManager *send_manager;
    Broadcaster *broadcaster, *send_broadcaster;



};

#endif // WIDGET_H
