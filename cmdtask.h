#ifndef CMDTASK_H
#define CMDTASK_H

#include <QDebug>
#include <QObject>
#include <QtMqtt/QtMqtt>
#include <QtMqtt/QMqttClient>
#include <QByteArray>
#include <QMqttTopicName>
#include <QTimer>

#include "tcpread.h"
#include "cmdquery_total_energy.h"
#include "cmdquery_energy_year.h"
#include "cmdquery_power_status.h"
#include "cmdquery_general_status.h"
#include "cmdquery_eminfo.h"

#include "configfile.h"

class CmdTask : public QObject
{
    Q_OBJECT

public:
    CmdTask(QObject *parent = nullptr);

    void init(ConfigOptions &configOptions) {
        tcpRead = new TcpRead(this);
        tcpRead->init(configOptions);

        cmdTimer = new QTimer(this);
        connect(cmdTimer, SIGNAL(timeout()), this, SLOT(onCmdTimer()));
        //cmdTimer->start(15000);
        cmdTimer->start(configOptions.waittime);

    }

public slots:
    void onCmdTimer() {
        currentListIndex++;
        if(currentListIndex>=cmdList.size()) {
            currentListIndex=0;
        }

        QByteArray topic = cmdList.at(currentListIndex)->getTopic();
        QByteArray payload = cmdList.at(currentListIndex)->resultToJson(
                    tcpRead->executeCommand(
                        cmdList.at(currentListIndex)->getCmd().constData())
                    );

        emit sReceivedData(topic, payload);

    }

signals:
    void sReceivedData(const QByteArray &topic, const QByteArray &payload);

protected:
    QTimer *cmdTimer;

    TcpRead *tcpRead;

    QList<iCmdQuery *> cmdList;
    int currentListIndex=0;

};

#endif // CMDTASK_H
