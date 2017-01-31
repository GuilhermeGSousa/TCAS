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

//    qDebug()<<"CheckSum";
//    qDebug()<<m.Ac_id;
//    qDebug()<<broadcaster->checksumCalc(buffer);
//    qDebug()<<m.CRC_32;
//    qDebug()<<"-------";
    scene_items->addIntruder(m);
    scene_items->updateIntruders();

}

void SceneManager::sendSelf()
{
    char *buff = new char[BUFFSIZE];
    QString valueInHex;

    uint32_t crc = broadcaster->checksumCalc(buff);
    scene_items->setCRC(crc);
    broadcaster->messageToBuffer(buff,scene_items->getSelf());

    int bytes= broadcaster->sendBuffer(buff);

}

