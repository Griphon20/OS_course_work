#include "mainwindow.h"
#include "ui_mainwindow.h"
int counter =1;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);
    nextBlockSize=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
  socket->connectToHost("127.0.0.1",2323);
  ui->pushButton->hide();
}

void MainWindow::slotReadyRead(){
   QDataStream in(socket);
   if(in.status()==QDataStream::Ok){
      /* QString str;
       in>>str;
       ui->textBrowser->append(str);*/
       for(;;)
       {

           if(nextBlockSize==0)
           {
               if(socket->bytesAvailable()<2)
               {
                   break;
               }
               in>>nextBlockSize;
           }
           if(socket->bytesAvailable()<nextBlockSize)
           {
               break;
           }

           QString name;
           QString str;

           in>>name>>str;
           nextBlockSize=0;
           if(counter==5)
           ui->textBrowser->append(" Я: "+str);
           else
           ui->textBrowser->append(" "+name+": "+str);
           counter =1;
       }
   }
else{
       ui->textBrowser->append("read error");
   }

}

void MainWindow::SendToServer(QString str,QString name){

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out <<quint16(0)<<name<<str;
    socket->write(Data);
    ui->lineEdit->clear();

}

void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text(),ui->lineEdit_2->text());
    counter =5;
}

void MainWindow::on_lineEdit_returnPressed()
{
     SendToServer(ui->lineEdit->text(),ui->lineEdit_2->text());
     counter =5;
}


