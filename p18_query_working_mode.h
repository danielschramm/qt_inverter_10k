#ifndef P18_QUERY_WORKING_MODE_H
#define P18_QUERY_WORKING_MODE_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include "response_enum.h"

class P18QueryWorkingMode : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D005"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

public:
    P18QueryWorkingMode(QString devName) : iCmdQuery("WorkingMode", devName) {
        QMap <int, QString> enumWorkingMode;
        enumWorkingMode[0]="Power on mode";
        enumWorkingMode[1]="Standby mode";
        enumWorkingMode[2]="Bypass mode";
        enumWorkingMode[3]="Battery mode";
        enumWorkingMode[4]="Fault mode";
        enumWorkingMode[5]="Hybrid mode(Line mode, Grid mode)";
        responseList.append(new ResponseEnum(devName, "WorkingMode", enumWorkingMode));
    }

    virtual QByteArray getCmd() {
        QByteArray cmd="^P006MOD";
        Crc crc;
        crc.appendCrc(cmd);
        cmd.append('\r');
        qDebug() << cmd;
        return(cmd);
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
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }
};

#endif // P18_QUERY_WORKING_MODE_H
