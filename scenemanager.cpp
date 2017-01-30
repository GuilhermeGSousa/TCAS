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
    scene_items->updateIntruders();
}

void SceneManager::sendSelf()
{
//    char *buff = new char[BUFFSIZE];
//    broadcaster->messageToBuffer(buff,scene_items->getSelf());
    qDebug()<<"Sending";
//    int bytes= broadcaster->sendBuffer(buff);
//    qDebug()<<bytes;
    broadcaster->sendBuffer("OLA!!!!!");
}

