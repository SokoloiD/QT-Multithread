#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include "qthread.h"
#include "buffer.h"

// класс - обработчик буфера
class Worker : public QThread
{
    Q_OBJECT
public:
    Worker(QObject *parent);
    ~Worker(){};
    void Init(TreadBuffer* bufferPtr);
    void run() override;

private:
    TreadBuffer*  buffer;
};

#endif // WORKER_H
