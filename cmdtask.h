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
#include "p17_query_total_energy.h"
#include "p17_query_energy_year.h"
#include "p17_query_power_status.h"
#include "p17_query_general_status.h"
#include "p17_query_eminfo.h"
#include "p17_cmd_battery_self_test.h"
#include "p18_query_general_status_list.h"
#include "p18_query_total_energy.h"
#include "p18_query_energy_year.h"
#include "p18_query_energy_month.h"
#include "p18_query_working_mode.h"

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

        baseTopic=configOptions.deviceName;
        if(QString::compare(configOptions.protocol,"p18", Qt::CaseInsensitive) == 0) {
            p18protocol=true;
        } else {
            p18protocol=false;
        }

        if(p18protocol) {
            cmdList.append(new P18QueryGeneralStatus(configOptions.deviceName));
            cmdList.append(new P18QueryTotalEnergy(configOptions.deviceName));
            cmdList.append(new P18QueryEnergyYear(configOptions.deviceName));
            cmdList.append(new P18QueryEnergyMonth(configOptions.deviceName));
            cmdList.append(new P18QueryWorkingMode(configOptions.deviceName));
        } else {
            cmdList.append(new P17QueryTotalEnergy(configOptions.deviceName));
            cmdList.append(new P17QueryEnergyYear(configOptions.deviceName));
            cmdList.append(new P17QueryPowerStatus(configOptions.deviceName));
            cmdList.append(new P17QueryGeneralStatus(configOptions.deviceName));
            cmdList.append(new P17QueryEmInfo(configOptions.deviceName));
            cmdList.append(new P17CmdBatterySelfTest(configOptions.deviceName));
        }
    }

public slots:
    void onCmdTimer() {
        currentListIndex++;
        if(currentListIndex>=cmdList.size()) {
            currentListIndex=0;
        }

        QByteArray payload = cmdList.at(currentListIndex)->resultToJson(
                    tcpRead->executeCommand(
                        cmdList.at(currentListIndex)->getCmd().constData())
                    );

        emit sMqttMessage(cmdList.at(currentListIndex)->getStateTopic().toUtf8(), payload, 0, false);
    }

    void onMqttConnect(QMqttClient *mClient) {
        foreach(auto c , cmdList) {
            foreach(auto r, c->getResponseList()) {
//                qDebug() << r->getAutodetectTopic();
//                qDebug() << r->getAutodetectPalyoad(c->getStateTopic());
                if(c->getCommandTopic().length()>0) {
                    auto subscription = mClient->subscribe(c->getCommandTopic(), 0);
                    connect(subscription, &QMqttSubscription::messageReceived,
                            c, &iCmdQuery::updateMessage);
                }
                emit sMqttMessage(r->getAutodetectTopic().toUtf8(),
                                  r->getAutodetectPalyoad(
                                      c->getStateTopic(),
                                      c->getAvailTopic(),
                                      c->getCommandTopic()
                                      ), 0, true);
            }
        }
    }

signals:
    void sMqttMessage(const QByteArray &topic, const QByteArray &payload, quint8 qos, bool retain);

protected:
    QTimer *cmdTimer;

    TcpRead *tcpRead;

    QList<iCmdQuery *> cmdList;
    int currentListIndex=0;

    bool p18protocol=false;

    QString baseTopic="";
};

#endif // CMDTASK_H
