#include "consumer.h"


TConsumer::TConsumer( QObject *parent): QThread(parent){}

void TConsumer::Init(TreadBuffer*  bufferPtr)
{     buffer = bufferPtr;

}
void TConsumer::run()
{
    char c;
    int id;
    while(buffer->IsBusyToConsumer()||buffer->isRunningState)
    {
        this->msleep(50);
        buffer->semElemsToRead.acquire(); //если есть элементы для обработки - берем. иначе ждем
        buffer->GetNextChar(c,id);
        emit  nextChar(c, id);// генерируем событие, что есть символ для обработки
        buffer->semEmptyCnt.release();//освобождаем место в буфере
    }
    emit jobDone(true);//уведомляем, что работа закончена
}
