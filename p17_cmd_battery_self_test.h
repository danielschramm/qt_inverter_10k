#ifndef CMDBATTERYSELFTEST_H
#define CMDBATTERYSELFTEST_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include <response_double.h>
#include <response_enum.h>
#include <response_longlong.h>
#include <response_switch.h>

class P17CmdBatterySelfTest : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

    bool doSend = false;

public:

    virtual QString getStateTopic() {
        QString stateTopic=devName + "/switch/" + cmdName + "/state";
        stateTopic.replace(QRegularExpression("\\s+"), "_");
        return (stateTopic);
    }
    virtual QString getCommandTopic() {
        QString stateTopic=devName + "/switch/" + cmdName + "/command";
        stateTopic.replace(QRegularExpression("\\s+"), "_");
        return (stateTopic);
    }

    P17CmdBatterySelfTest(QString devName) : iCmdQuery("BatteryTest", devName) {
        responseList.append(new ResponseSwitch(devName, "BatteryTest"));
    }

    virtual QByteArray getCmd() {
        if(doSend) {
            qDebug() << "BatteryTest acivated.";
            doSend=false;
            return("^S004BST\r");
        } else {
            return("");
        }
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);
            responseList.at(0)->setValue(temp);

            QJsonObject  recordObject;
            foreach (auto entry, responseList) {
                recordObject.insert(entry->getJsonKey(), entry->getJsonValue());
            }
            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format xxxx";
            return("");
        }
        return("");
    }

    virtual void updateMessage(const QMqttMessage &msg) {
        if(msg.payload().compare("ON") == 0) {
            doSend = true;
        }
    }

};

#endif // CMDBATTERYSELFTEST_H
