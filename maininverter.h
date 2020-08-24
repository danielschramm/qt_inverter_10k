#ifndef MAININVERTER_H
#define MAININVERTER_H

#include<QDebug>
#include <QObject>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include <QByteArray>
#include <QMqttTopicName>
#include <QTimer>

#include "tcpread.h"
#include "cmdquery_total_energy.h"

#include "configfile.h"


class MainInverter : public QObject
{
    Q_OBJECT

public:
    explicit MainInverter(QObject *parent = nullptr);

    void init(ConfigOptions &configOptions) {
        m_client = new QMqttClient(this);
        m_client->setClientId("qt_inverter_10k");
        m_client->setHostname(configOptions.mqttServerName);
        m_client->setPort(configOptions.mqttServerPort);
        m_client->setUsername(configOptions.mqttServerUser);
        m_client->setPassword(configOptions.mqttServerPass);
        //m_client->setAutoKeepAlive(true);

        QObject::connect(m_client, &QMqttClient::connected, this, &MainInverter::onConnected);
        //QObject::connect(&_timer, &QTimer::timeout, this, &Publisher::onTimeout);
        QObject::connect(m_client, &QMqttClient::disconnected, this, &MainInverter::onDisconnected);
        //QObject::connect(m_client,&QMqttClient::error, this, &MainInverter::onErrer);
        //QObject::connect(m_client, &QMqttClient::received, this, &MainInverter::onReceived);

        m_client->connectToHost();

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
        timer->start(30000);
    }


signals:
    void finished();

public slots:

    void onConnected() {
        qDebug() << "Connected to mqtt broker ";
        statusConnected=true;
    }

    void onDisconnected() {
        qDebug() << "Disconnected from mqtt broker";
        statusConnected=false;
    }

    void onError()
    { qDebug() << "onError"; }

    void onReceived()
    { qDebug() << "onReceived"; }


    void onTimer() {
        //        emit finished();
        if( ! statusConnected ) {
            m_client->connectToHost();
        }
    }

    void onReceivedData(const QByteArray &topic, const QByteArray &payload) {
        qDebug() << "received data";
        /*
        if(topic.size()==0)
            return;
        if(payload.size()==0)
            return;
        */
        if(m_client->state() == QMqttClient::ClientState::Connected) {

            if(m_client->publish(QString(topic), payload) == -1) {
                qDebug() << "publish error";
            } else {
                qDebug() << "publish";
            }
        }

    }

signals:

protected:
    QMqttClient *m_client;
    QTimer *timer;

    bool statusConnected=false;

};

#endif // MAININVERTER_H