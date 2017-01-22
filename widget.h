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
    QTimer *time;
    SceneManager *scene_manager;
    Broadcaster *broadcaster;



};

#endif // WIDGET_H
