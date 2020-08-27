#ifndef CMDQUERYTOTALENERGY_H
#define CMDQUERYTOTALENERGY_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include <response_double.h>
#include <response_enum.h>
#include <response_longlong.h>

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
    P17QueryTotalEnergy(QString devName) : iCmdQuery("TotalEnergy", devName) {
        responseList.append(new ResposeLongLong(devName, "TotalEnergy", "kWh"));
    }

    virtual QByteArray getCmd() {
        return("^P003ET\r");
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

#endif // CMDQUERYTOTALENERGY_H
