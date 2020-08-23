#include "producer.h"
#include "buffer.h"

Producer::Producer( QObject *parent): QThread(parent){}

void Producer::Init(QSharedPointer<QFile> fileptr, int id, TreadBuffer* bufferPtr)
{
    buffer = bufferPtr;
    TreadID = id;
    file =   fileptr;
    totalRead =0;
    totalLen= file->size();
}
// собсвенно основная процедура
void Producer::run()
{   char c;
    //читаем файл посимвольно с задержкой
    while ((!file->atEnd())&&buffer->isRunningState)
    {   totalRead++;
        this->msleep(200);//задержка в 300 мсек
        file->getChar(&c);
        //уменьшаем количество доступных элементов в буфере (через семафор elemsToWrite)
        // если места для записи нет - ждем
        buffer->semEmptyCnt.acquire();
        buffer->PutNextChar(c,this->TreadID);
        buffer->semElemsToProcess.release(); //увеличиваем семафор для обработки
        emit readProgress(int(100*totalRead/totalLen),TreadID);
     }
    buffer->ProducerDone();
}
