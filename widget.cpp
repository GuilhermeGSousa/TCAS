#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->downButton->setVisible(false);
    ui->leftButton->setVisible(false);
    ui->rightButton->setVisible(false);
    ui->upButton->setVisible(false);
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

    ui->graphicsView->setScene(scene);
    whitePen.setWidth(10);
    scene->setBackgroundBrush(greyBrush);
    radius=totalHeight*0.9/2;

    scene_items = new SceneItems(totalWidth,totalHeight,radius);
    scene->addItem(scene_items);

    scene_manager=new SceneManager(scene_items,broadcaster);
}

void Widget::setupListener(int portNum)
{
    broadcaster = new Broadcaster(portNum);


    time = new QTimer(this);
    QThread* listener = new QThread;
    scene_manager->moveToThread(listener);
    broadcaster->moveToThread(listener);

    connect(listener,SIGNAL(started()),broadcaster,SLOT(listenBuffer()));
    connect(broadcaster,SIGNAL(messageReceived(char*)),scene_manager,SLOT(updateScene(char*)));
    connect(time,SIGNAL(timeout()),scene,SLOT(advance()));
    time->start(10);
    listener->start();
}







Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    //Eu depois limpo isto
    //Mas limpa mesmo Gui isto parece o teu quarto
    if(ui->lineEdit->text().size()==0)
        return;
    else{
        setup();
        setupListener(ui->lineEdit->text().toInt());
        ui->pushButton->setVisible(false);
        ui->label->setVisible(false);
        ui->lineEdit->setVisible(false);
        ui->downButton->setVisible(true);
        ui->leftButton->setVisible(true);
        ui->rightButton->setVisible(true);
        ui->upButton->setVisible(true);
    }

}



void Widget::on_leftButton_pressed()
{

}

void Widget::on_downButton_pressed()
{
    scene_items->goDown();
}

void Widget::on_rightButton_pressed()
{

}

void Widget::on_upButton_pressed()
{
    scene_items->goUp();
}


//ON Click callbacks

void Widget::on_leftButton_clicked()
{

}

void Widget::on_rightButton_clicked()
{

}

void Widget::on_upButton_clicked()
{

}

void Widget::on_downButton_clicked()
{

}


