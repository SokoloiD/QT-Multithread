#include "multithreadwindow.h"
#include "ui_multithreadwindow.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qdebug.h"
#include "buffer.h"
#include "consumer.h"
#include "worker.h"
#include "producer.h"



MultiThreadWindow::MultiThreadWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MultiThreadWindow)
{
    ui->setupUi(this);
    tBuffer = new TreadBuffer();
    consumer = new TConsumer( this);
    producer1  = new Producer(this);
    producer2  = new Producer(this);
    producer3  = new Producer(this);
    worker = new Worker(this);
    ui->labelFile1->setStyleSheet("background: #C0FFFF;");
    ui->labelFile2->setStyleSheet("background: #FFC0FF;");
    ui->labelFile3->setStyleSheet("background: #FFFFC0;");
    ui->textEdit1->setStyleSheet("background: #C0FFFF;");
    ui->textEdit2->setStyleSheet("background: #FFC0FF;");
    ui->textEdit3->setStyleSheet("background: #FFFFC0;");
    connect(consumer, SIGNAL(nextChar(char,int)), SLOT(addChar(char,int)));
    connect(consumer, SIGNAL(jobDone(bool)), SLOT(blockButtons(bool)));
    connect(producer1, SIGNAL(readProgress( int , int )), SLOT(updateProgressBar(int , int )));
    connect(producer2, SIGNAL(readProgress( int , int )), SLOT(updateProgressBar(int , int )));
    connect(producer3, SIGNAL(readProgress( int , int )), SLOT(updateProgressBar(int , int )));
}

MultiThreadWindow::~MultiThreadWindow()
{

    delete producer1;
    delete producer2;
    delete producer3;
    delete worker;
    delete tBuffer;
    delete ui;

}
//выбор имени файла
QString MultiThreadWindow::getFileName()
{
    QFileDialog dialog(this);
    QStringList fileNames;
    dialog.setFileMode(QFileDialog::ExistingFile);
    if (dialog.exec())
    {
        return dialog.selectedFiles().at(0);
    }
    else
    {
        return "";
    }
}

void MultiThreadWindow::on_pushButtonFile1_clicked()
{
     ui->labelFile1->setText(this->getFileName());

}

void MultiThreadWindow::on_pushButtonFile2_clicked()
{
    ui->labelFile2->setText(this->getFileName());
}

void MultiThreadWindow::on_pushButtonFile3_clicked()
{
      ui->labelFile3->setText(this->getFileName());
}
//старт многопоточных вычислений
void MultiThreadWindow::on_pushButton_clicked()
{

    //открываем файлы
     QSharedPointer<QFile> file1 = QSharedPointer<QFile>::create(ui->labelFile1->text());
     QSharedPointer<QFile> file2 = QSharedPointer<QFile>::create(ui->labelFile2->text());
     QSharedPointer<QFile> file3 = QSharedPointer<QFile>::create(ui->labelFile3->text());

     if (!file1->open(QFile::ReadOnly)) {
       QMessageBox::critical(this, "Error", QString("Ошибка открытия файла \"%1\": %2").arg(file1->fileName(), file1->errorString()));
       return;
     }

     if (!file2->open(QFile::ReadOnly)) {
       QMessageBox::critical(this, "Error", QString("Ошибка открытия файла file \"%1\": %2").arg(file2->fileName(), file2->errorString()));
       return;
     }

     if (!file3->open(QFile::ReadOnly)) {
       QMessageBox::critical(this, "Error", QString("Ошибка открытия файла  \"%1\": %2").arg(file3->fileName(), file3->errorString()));
       return;
     }


     //если открытие успешно - блокируем кнопки и  чистим окна вывода
     blockButtons(false);

     ui->textEdit1->clear();
     ui->textEdit2->clear();
     ui->textEdit3->clear();
     // инициализируем объекты
      tBuffer->Init(ui->spinBoxBufferSize->value(), 3);
      consumer->Init(tBuffer);
      producer1->Init(file1,1,tBuffer);
      producer2->Init(file2,2,tBuffer);
      producer3->Init(file3,3,tBuffer);
      worker->Init(tBuffer);





      producer1->start();
      producer2->start();
      producer3->start();

      worker->start();

      consumer->start();



}
//остановка многопоточных вычислений
void MultiThreadWindow::on_pushButton_2_clicked()
{
    tBuffer->isRunningState = false;
}
void MultiThreadWindow::addChar(char c, int id)
{
    //if (id==1) qDebug() << "addChar id "<< id << c;
    switch (id)
    {
        case 1:
            ui->textEdit1->insertPlainText(QChar(c));
            break;
        case 2:
            ui->textEdit2->insertPlainText(QChar(c));
            break;
        case 3:
            ui->textEdit3->insertPlainText(QChar(c));
            break;
    }
    ui->textEditBuffer->setHtml(tBuffer->GetColoredHTMLText());

}

void MultiThreadWindow::on_pushButton_3_clicked()
{

}
void MultiThreadWindow::blockButtons(bool block)
{   qDebug() << "Block set   "<< block ;
    ui->pushButtonFile1->setEnabled(block);
    ui->pushButtonFile2->setEnabled(block);
    ui->pushButtonFile3->setEnabled(block);
    ui->pushButton->setEnabled(block);
    ui->pushButton_3->setEnabled(block);
}

void MultiThreadWindow::updateProgressBar(int percent, int id)
{   qDebug() << "Progress id "<< id << "  "<<percent;
    switch (id)
        {
            case 1:
                ui->progressBar1->setValue(percent);
                break;
            case 2:
                ui->progressBar2->setValue(percent);
                break;
            case 3:
                ui->progressBar3->setValue(percent);
                break;
        }
    }

