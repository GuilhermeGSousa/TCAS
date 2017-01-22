#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include "broadcaster.h"

class SceneManager : public QObject
{
    Q_OBJECT
public:
    SceneManager(QGraphicsScene*,Broadcaster*);

public slots:
    void updateScene(char*);
signals:
private:
    QGraphicsScene* scene;
    Broadcaster* broadcaster;
};

#endif // SCENEMANAGER_H
