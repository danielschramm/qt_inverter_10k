#ifndef TCPREAD_H
#define TCPREAD_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QByteArray>
#include "configfile.h"

class TcpRead : public QObject
{
    Q_OBJECT
public:
    explicit TcpRead(QObject *parent = nullptr);

    void init(ConfigOptions &_configOptions) {
        configOptions=_configOptions;

        socket = new QTcpSocket(this);

        connect(socket, SIGNAL(connected()),this, SLOT(connected()));
        connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
        connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
        connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

        doConnect();
    }


signals:

public slots:
    void connected() {
        qDebug() << "connected...";
        connectionState=true;
    }

    void disconnected() {
        connectionState=false;
        qDebug() << "disconnected...";
    }

    void bytesWritten(qint64 bytes) {
        qDebug() << bytes << " bytes written...";
    }
    void readyRead() {
        qDebug() << "ignore reading...";
    }


    QByteArray executeCommand(const char *command) {

        if(!connectionState) {
            doConnect();
        }

        socket->write(command);
        socket->waitForReadyRead(2000);
         QByteArray rData = socket->readAll();
         qDebug() << rData;
         return(rData);
    }


private:
    ConfigOptions configOptions;
    QTcpSocket *socket;

    bool connectionState=false;

    void doConnect() {
        qDebug() << "connecting...";

        // this is not blocking call
        socket->connectToHost(configOptions.serialServerName, configOptions.serialServerPort);

        // we need to wait...
        if(!socket->waitForConnected(5000))
        {
            qDebug() << "Error: " << socket->errorString();
        }
    }


};

#endif // TCPREAD_H
