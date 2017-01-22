#include "scenemanager.h"
#include <QDebug>


SceneManager::SceneManager(SceneItems *scene_input,Broadcaster *b)
{
    scene_items = scene_input;
    broadcaster = b;
}

void SceneManager::updateScene(char *buffer)
{
    Message m = broadcaster->bufferToMessage(buffer);
    scene_items->addIntruder(m);

    qDebug()<<m.X_pos;
}

