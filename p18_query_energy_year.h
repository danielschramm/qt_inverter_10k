#ifndef P18_QUERY_ENERGY_YEAR_H
#define P18_QUERY_ENERGY_YEAR_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>

class P18QueryEnergyYear : public iCmdQuery
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
    P18QueryEnergyYear() {

    }

    virtual QByteArray getCmd() {
        QByteArray cmd = "^P009EY";
        QByteArray year;
        year.setNum(QDate::currentDate().year());
        cmd.append(year);

        Crc crc;
        crc.appendCrc(cmd);

        cmd.append("\r");
        return(cmd);
    }

    virtual QByteArray getTopic() {
        return("/YearlyEnergy");
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);
            qlonglong energy =  temp.toLongLong();
            qDebug() << "result: " << energy;


            QJsonObject  recordObject;
            recordObject.insert("YearlyEnergy", QJsonValue::fromVariant(energy));
            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};

#endif // P18_QUERY_ENERGY_YEAR_H
