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

    qDebug()<<"CheckSum";
    qDebug()<<m.Ac_id;
    qDebug()<<broadcaster->checksumCalc(buffer);
    qDebug()<<m.CRC_32;
    qDebug()<<"-------";
    scene_items->addIntruder(m);
    scene_items->updateIntruders();

}

void SceneManager::sendSelf()
{
    char *buff = new char[BUFFSIZE];

    broadcaster->messageToBuffer(buff,scene_items->getSelf());
    uint64_t crc = broadcaster->checksumCalc(buff);
    memcpy(buff+BUFFSIZE-5,(uint64_t*)&crc,sizeof(crc));
    int bytes= broadcaster->sendBuffer(buff);
//    qDebug()<<bytes;
}

