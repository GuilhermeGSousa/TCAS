#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QObject>
#include <QGraphicsScene>
#include "sceneitems.h"
#include "broadcaster.h"

class SceneManager : public QObject
{
    Q_OBJECT
public:
    SceneManager(SceneItems*,Broadcaster*);

public slots:
    void updateScene(char*);
signals:
private:
    SceneItems* scene_items;
    Broadcaster* broadcaster;
};

#endif // SCENEMANAGER_H
