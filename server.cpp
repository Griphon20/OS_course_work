#include <iostream>
#include "server.h"

server::server()
{
if(this->listen(QHostAddress::Any,2323))
{
    qDebug()<<"start";
}
else
{
    qDebug()<<"fail";
}
 nextBlockSize=0;
}
void server :: incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket,&QTcpSocket::readyRead,this,&server::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected,this,&QTcpSocket::deleteLater);

    Sockets.push_back(socket);
        qDebug()<<"client connected"<<socketDescriptor;
}
void server::slotReadyRead(){

    socket =(QTcpSocket*)sender();
    QDataStream in(socket);
    if(in.status()==QDataStream::Ok)
    {
        qDebug()<<"read...";
       /* QString str;
        in >>str;
        qDebug() <<str;
        SendToClient(str);*/
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
            QString str;
            QString name;
            QTime time;
            in >> time >>name>> str;
            nextBlockSize=0;
            SendToClient(str,name);
            break;
        }
    }
    else
    {
        qDebug()<<"DataStream error";
    }
}

void server::SendToClient(QString str,QString name){

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out << quint16(0) << QTime::currentTime()<< name<< str;
    out.device()->seek(0);
    out<< quint16(Data.size()-sizeof(quint16));
    // socket->write(Data);
     for(int i=0; i<Sockets.size();i++){
         Sockets[i]->write(Data);
     }
}
