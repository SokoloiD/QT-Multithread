#ifndef PRODUCER_H
#define PRODUCER_H
#include "QThread"
#include "buffer.h"
#include "QFile"
#include "QString"
#include "QSharedPointer"

//класс задачи для чтения файла
class Producer : public QThread
{
    Q_OBJECT
public:
    Producer(QObject *parent);
    void Init(QSharedPointer<QFile> file, int id, TreadBuffer* bufferPtr);
    void run() override;
signals:
     void readProgress( int percent, int id);
private:
    TreadBuffer *buffer;
    int TreadID;
    QSharedPointer<QFile> file;
    int totalRead;
    int totalLen;



};

#endif // PRODUCER_H
