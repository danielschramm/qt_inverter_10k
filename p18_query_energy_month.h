#ifndef P18_QUERY_ENERGY_MONTH_H
#define P18_QUERY_ENERGY_MONTH_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include <response_double.h>
#include <response_enum.h>
#include <response_longlong.h>
#include <QDate>

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
    P18QueryEnergyMonth(QString devName) : iCmdQuery("MonthlyEnergy", devName) {
        responseList.append(new ResposeLongLong(devName, "MonthlyEnergy", "Wh"));
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

#endif // P18_QUERY_ENERGY_MONTH_H
