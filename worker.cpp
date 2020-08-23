#include "worker.h"

Worker::Worker( QObject *parent): QThread(parent){}

void Worker::Init(TreadBuffer* bufferPtr)
{
    buffer = bufferPtr;
}
void Worker::run()
{   //int charsProcessedCnt =0; //счетчик обработанных символов
    char c;
    while(buffer->IsBusyToWorker()){
        buffer->semElemsToProcess.acquire(); //если есть элементы для обработки - берем. иначе ждем
        c = buffer->GetProcessChar();
        //обрабатываем символ. четные в нижний регистр, нечетные в верхний
        if (c%2)
           c = tolower(c);
        else
            c= toupper(c);
        this->msleep(100);//0.1 секунды эмулируем напряженную работу
        buffer->SetProcessChar(c);
        buffer->semElemsToRead.release();//освобождаем семафор для чтения обработанного символа
    }
}
