#ifndef MULTITHREADWINDOW_H
#define MULTITHREADWINDOW_H


#include <QMainWindow>
#include "QStringList"
#include "buffer.h"
#include "consumer.h"
#include "worker.h"
#include "producer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MultiThreadWindow; }
QT_END_NAMESPACE

class MultiThreadWindow : public QMainWindow
{
    Q_OBJECT

public:
    MultiThreadWindow(QWidget *parent = nullptr);
    ~MultiThreadWindow();


private slots:
    void addChar(char c, int id);//направляет символ в нужный TextEdi
    void updateProgressBar(int percent, int ID);
    QString  getFileName();
    void on_pushButtonFile1_clicked();

    void on_pushButtonFile2_clicked();

    void on_pushButtonFile3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void blockButtons(bool block);

private:
    Ui::MultiThreadWindow *ui;
    TreadBuffer*  tBuffer;//указатель на буфер

    Producer *producer1;
    Producer *producer2;
    Producer *producer3;
    TConsumer *consumer;
    Worker *worker;



};
#endif // MULTITHREADWINDOW_H
