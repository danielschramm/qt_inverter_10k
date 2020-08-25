#ifndef CMDQUERYTOTALENERGY_H
#define CMDQUERYTOTALENERGY_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>

class P17QueryTotalEnergy : public iCmdQuery
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
    P17QueryTotalEnergy() {

    }

    virtual QByteArray getCmd() {
        return("^P003ET\r");
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

#endif // CMDQUERYTOTALENERGY_H
