#ifndef P18_QUERY_ENERGY_YEAR_H
#define P18_QUERY_ENERGY_YEAR_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include <QDate>
#include <response_double.h>
#include <response_enum.h>
#include <response_longlong.h>

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
    P18QueryEnergyYear(QString devName) : iCmdQuery("YearlyEnergy", devName) {
       responseList.append(new ResposeLongLong(devName, "YearlyEnergy", "Wh"));
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

#endif // P18_QUERY_ENERGY_YEAR_H
