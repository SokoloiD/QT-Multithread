#include "buffer.h"
#include "qdebug.h"


TreadBuffer::TreadBuffer()
{
    Buffer= nullptr;
    isRunningState=false;

}
TreadBuffer::~TreadBuffer()
{
    if (Buffer)
        delete Buffer;
}

void TreadBuffer::Init(int buffersize, int producercnt)
{
    if (Buffer)
        delete Buffer;
    bufferSize = buffersize;
    Buffer= new BufferElem[bufferSize];
     // сколько можно прочитать из буфера
    writtenCharsCnt =0; // сколько всего байтов было записано
    readedCharsCnt =0 ;// сколько всего байтов было прочитано
    processCharsCnt = 0;//сколько было обработано
    producersCnt=producercnt;
    semEmptyCnt.acquire(semEmptyCnt.available());
    semEmptyCnt.release(bufferSize) ;// сколько есть места под запись
    semElemsToRead.acquire(semElemsToRead.available()); // сколько элеметов для чтения
    semElemsToProcess.acquire(semElemsToProcess.available());
    isRunningState = true;

}
void TreadBuffer::GetNextChar(char &c, int &id)
{
    QMutexLocker lock(&bufferMutex);
    //if (id==1) qDebug() << "GetNextChar id "<< id << c;

    c = Buffer[readedCharsCnt%bufferSize].chr;
    id = Buffer[readedCharsCnt%bufferSize].treadID;
    readedCharsCnt ++;
}

void TreadBuffer::PutNextChar(char c, int id)
{   QMutexLocker lock(&bufferMutex);// локировка мьютекса, разлокировка по выходу из области видимости
    //if (id==1) qDebug() << "PutNextChar id "<< id << c;
    Buffer[writtenCharsCnt%bufferSize].chr = c;
    Buffer[writtenCharsCnt%bufferSize].treadID =id;
    writtenCharsCnt ++;

}
char TreadBuffer::GetProcessChar()//дает последний необработанный символ
{   QMutexLocker lock(&bufferMutex);
    //if(Buffer[processCharsCnt%bufferSize].treadID==1) qDebug() << "GetProcessChar   "<<  processCharsCnt;
    return Buffer[processCharsCnt%bufferSize].chr;


}
void TreadBuffer::SetProcessChar(char c) //кладет обработанный символ и увеличивает счетчик
{
      QMutexLocker lock(&bufferMutex);
      //if(Buffer[processCharsCnt%bufferSize].treadID==1) qDebug() << "SetProcessChar   "<<  processCharsCnt<<c;
      Buffer[processCharsCnt%bufferSize].chr = c;
      processCharsCnt++;
}
QString TreadBuffer::GetColoredHTMLText()
{
    QString strHTML = "";
    QString tmp ="";
    QMutexLocker lock(&bufferMutex);
    //формируем строку html на основе ID выбираем цвета
    for (int   i =  ((0 > readedCharsCnt) ? 0 : readedCharsCnt) ;i<writtenCharsCnt;i++)
    {
        strHTML += "<span style=\"background:#";
        switch (Buffer[i%bufferSize].treadID)
        {
          case 1:
            strHTML += "C0FFFF";
            break;

          case 2:
            strHTML += "FFC0FF";
            break;

          case 3:
            strHTML += "FFFFC0";
            break;
          }
          strHTML += ";\">[";
          // если символ совпадает с символом для обработки -выделяем его жирным
          if (i ==processCharsCnt )
            strHTML += "<b>";

          tmp =QChar(Buffer[i%bufferSize].chr);
          strHTML += tmp.toHtmlEscaped();

          if (i == processCharsCnt)
            strHTML += "</b>";

          strHTML += "]</span> ";
        }
    return strHTML;
}
