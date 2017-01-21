#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    plane = QPixmap::fromImage(QImage(":/plane"));
    intruder = QPixmap::fromImage(QImage(":/rhombus"));
    scale=0.3;

    time = new QTimer(this);
    connect(time,SIGNAL(timeout()),scene,SLOT(advance()));
    time->start(10);
}

void Widget::paintEvent(QPaintEvent *event)
{

}

void Widget::resizeEvent(QResizeEvent *event){

    totalWidth = ui->graphicsView->width();
    totalHeight = ui->graphicsView->height();

}

void Widget::setup(){
    QBrush greyBrush(Qt::gray);
    QBrush blackBrush(Qt::black);
    QPen whitePen(Qt::white);


    intr = scene->addPixmap(intruder);
    ui->graphicsView->setScene(scene);
    whitePen.setWidth(10);
    scene->setBackgroundBrush(greyBrush);
    radius=totalHeight*0.9/2;
    scene->addEllipse(totalWidth/2-radius,totalHeight/2-radius,radius*2,radius*2,whitePen,greyBrush);
    pln = scene->addPixmap(plane);
    pln->setScale(scale);
    pln->setPos(totalWidth/2-plane.width()*(scale/2),totalHeight/2+plane.height()*scale*1.5);

    Pointer *point = new Pointer(totalWidth,totalHeight,radius);
    scene->addItem(point);
}







Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    //Eu depois limpo isto
    //Mas limpa mesmo Gui isto parece o teu quarto

    setup();
    ui->pushButton->setVisible(false);


}



