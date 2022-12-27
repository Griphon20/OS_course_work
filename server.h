#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QVector>
#include<QDataStream>
#include<QTime>
class server:public QTcpServer
{
    Q_OBJECT
public:
    server();
    QTcpSocket *socket;
private:
    QVector <QTcpSocket*> Sockets;
    QByteArray Data;
    void SendToClient(QString str,QString name);
    quint16 nextBlockSize;

private slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
};

#endif // SERVER_H
