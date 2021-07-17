#ifndef ICMD_H
#define ICMD_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <crc.h>
#include "iResponse.h"
#include <qmqttmessage.h>

class iCmdQuery: public QObject
{
protected:
    QString cmdName;
    QString devName;
    QList<iResponse *> responseList;

public:

    iCmdQuery(QString cmdName, QString devName) : cmdName(cmdName), devName(devName) {

    }

    virtual QByteArray getCmd() = 0;

    //virtual QByteArray getTopic() = 0;

    virtual QByteArray resultToJson(QByteArray response) = 0;

    virtual QString getStateTopic() {
        QString stateTopic=devName + "/sensor/" + cmdName + "/state";
        stateTopic.replace(QRegularExpression("\\s+"), "_");
        return (stateTopic);
    }
    virtual QString getAvailTopic() {
        return ("");
    }
    virtual QString getCommandTopic() {
        return ("");
    }

    QString getCommandName() {
        return cmdName;
    }

    QList<iResponse *>& getResponseList() {
        return responseList;
    }

    virtual void updateMessage(const QMqttMessage &msg) {
        qDebug() << msg.topic();
    }

};

#endif // ICMD_H
