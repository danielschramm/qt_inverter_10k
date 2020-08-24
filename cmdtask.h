#ifndef CMDTASK_H
#define CMDTASK_H

#include <Qt>
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
#include "p18_query_general_status.h"
#include "p18_query_total_energy.h"
#include "p18_query_energy_year.h"
#include "p18_query_energy_month.h"

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

        baseTopic=configOptions.mqttTopic;
        if(QString::compare(configOptions.protocol,"p18", Qt::CaseInsensitive) == 0) {
            p18protocol=true;
        } else {
            p18protocol=false;
        }

        if(p18protocol) {
            cmdList.append(new P18QueryGeneralStatus);
            cmdList.append(new P18QueryTotalEnergy);
            cmdList.append(new P18QueryEnergyYear);
            cmdList.append(new P18QueryEnergyMonth);
        } else {
            cmdList.append(new CmdQueryTotalEnergy);
            cmdList.append(new CmdQueryEnergyYear);
            cmdList.append(new CmdQueryPowerStatus);
            cmdList.append(new CmdQueryGeneralStatus);
            cmdList.append(new CmdQueryEmInfo);
        }

    }

public slots:
    void onCmdTimer() {
        currentListIndex++;
        if(currentListIndex>=cmdList.size()) {
            currentListIndex=0;
        }

        QByteArray topic = baseTopic.toUtf8();
        topic.append(cmdList.at(currentListIndex)->getTopic());
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

    bool p18protocol=false;

    QString baseTopic="";
};

#endif // CMDTASK_H
