#ifndef P18_QUERY_ENERGY_MONTH_H
#define P18_QUERY_ENERGY_MONTH_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>

class P18QueryEnergyMonth : public iCmdQuery
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
    P18QueryEnergyMonth() {

    }

    virtual QByteArray getCmd() {
        QByteArray cmd = "^P011EM";
        QByteArray year;
        year.setNum(QDate::currentDate().year());
        cmd.append(year);
        QByteArray month = QDate::currentDate().toString("MM").toUtf8();
        cmd.append(month);

        Crc crc;
        crc.appendCrc(cmd);

        cmd.append("\r");
        return(cmd);
    }

    virtual QByteArray getTopic() {
        return("/MonthlyEnergy");
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);
            qlonglong energy =  temp.toLongLong();
            qDebug() << "result: " << energy;


            QJsonObject  recordObject;
            recordObject.insert("MonthlyEnergy", QJsonValue::fromVariant(energy));
            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};

#endif // P18_QUERY_ENERGY_MONTH_H
