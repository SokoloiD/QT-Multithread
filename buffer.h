
#include "qsemaphore.h"
#include "qmutex.h"
#include "qsharedpointer.h"

#ifndef BUFFER_H
#define BUFFER_H

//элемент буфера. содержит символ и ID потока, который записал его в буфер
struct BufferElem{
    char chr;
    int treadID;
};

class TreadBuffer{

public:
    QSemaphore semEmptyCnt ;// сколько есть места под запись
    QSemaphore semElemsToRead; // сколько элеметов для чтения
    QSemaphore semElemsToProcess; // сколько элеметов для обработки

    TreadBuffer();
    ~TreadBuffer();
    void Init(int BufferSize, int producercnt);

    void GetNextChar(char &c, int &id); //читаем из буфера
    void PutNextChar(char c, int id);// пишем в буфер
    char GetProcessChar();//дает последний необработанный символ
    void SetProcessChar(char);//кладет обработанный символ и увеличивает счетчик обработанных данных
    bool IsBusyToWorker(){// если есть подключенииые Producers или буфер не пустой для обработки
        return (producersCnt)||(processCharsCnt<writtenCharsCnt);
    };
    bool IsBusyToConsumer(){// если есть подключенииые Producers или буфер не пустой для чтения
        return (producersCnt)||(readedCharsCnt<writtenCharsCnt);
    };
    void SetProducersCnt(int cnt){producersCnt =cnt;};
    void ProducerDone(){producersCnt--;};
    QString GetColoredHTMLText();
    bool isRunningState;

private:
    int writtenCharsCnt; //сколько всего байтов было записано
    int readedCharsCnt; //сколько всего байтов было прочитано
    int processCharsCnt; //сколько всего байтов было прочитано
    BufferElem *Buffer;
    int bufferSize;
    int producersCnt;//число подключенных потоко - продюсеров
    QMutex bufferMutex;
};

#endif // BUFFER_H
