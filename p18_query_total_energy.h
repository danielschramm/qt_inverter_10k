#ifndef P18_TOTAL_ENERGY_H
#define P18_TOTAL_ENERGY_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>

class P18QueryTotalEnergy : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D011"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

public:
    P18QueryTotalEnergy() {

    }

    virtual QByteArray getCmd() {
        QByteArray cmd="^P005ET";
        Crc crc;
        crc.appendCrc(cmd);
        cmd.append('\r');
        qDebug() << cmd;
        return(cmd);
    }

    virtual QByteArray getTopic() {
        return("/TotalEnergy");
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);
            qlonglong totalEnergy =  temp.toLongLong();
            qDebug() << "result: " << totalEnergy;


            QJsonObject  recordObject;
            recordObject.insert("TotalEnergy", QJsonValue::fromVariant(totalEnergy));
            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};


#endif // P18_TOTAL_ENERGY_H
