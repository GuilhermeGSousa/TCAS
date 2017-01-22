#include "scenemanager.h"
#include <QDebug>


SceneManager::SceneManager(QGraphicsScene *scene_input,Broadcaster *b)
{
    scene = scene_input;
}

void SceneManager::updateScene(char *buffer)
{
    Message m = broadcaster->bufferToMessage(buffer);
    qDebug()<<m.X_pos;
}

