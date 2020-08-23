#ifndef CONSUMER_H
#define CONSUMER_H
#include "QThread"
#include "buffer.h"


class TConsumer : public QThread
{
    Q_OBJECT
public:
    TConsumer( QObject *parent);
    ~TConsumer(){};
    void Init(TreadBuffer*  bufferPtr);
    void run() override;

signals:
     void nextChar( char c,int id);
     void jobDone(bool state);
private:
     TreadBuffer*  buffer;
};

#endif // CONSUMER_H
