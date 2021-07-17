#ifndef CMDQUERYfpadj_H
#define CMDQUERYfpadj_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include <response_double.h>
#include <response_enum.h>
#include <response_longlong.h>

class P17QueryEmInfo : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D030"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

public:
    P17QueryEmInfo(QString devName) : iCmdQuery("EmInfo", devName) {
        responseList.append(new ResponseDouble(devName, "emInfo_tbd", 1.0, "W"));
        responseList.append(new ResponseDouble(devName, "emInfo_default_feedin_power", 1.0, "W"));
        responseList.append(new ResponseDouble(devName, "emInfo_actual_solar_power", 1.0, "W"));
        responseList.append(new ResponseDouble(devName, "emInfo_actual_feedin_power", 1.0, "W"));
        responseList.append(new ResponseDouble(devName, "emInfo_actual_reserved_hybrid_power", 1.0, "W"));
        responseList.append(new ResponseDouble(devName, "emInfo_tbd2", 1.0, "W"));
    }

    virtual QByteArray getCmd() {
        return("^P007EMINFO\r");
    }

    /*
    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);

            QList<QByteArray> elements = temp.split(',');

            QJsonObject  recordObject;
            qlonglong value;

            value = elements.at(0).toLongLong();
            recordObject.insert("tbd", QJsonValue::fromVariant(value));
            value = elements.at(1).toLongLong();
            recordObject.insert("default_feedin_power", QJsonValue::fromVariant(value));
            value = elements.at(2).toLongLong();
            recordObject.insert("actual_solar_power", QJsonValue::fromVariant(value));
            value = elements.at(3).toLongLong();
            recordObject.insert("actual_feedin_power", QJsonValue::fromVariant(value));
            value = elements.at(4).toLongLong();
            recordObject.insert("actual_reserved_hybrid_power", QJsonValue::fromVariant(value));
            value = elements.at(5).toLongLong();
            recordObject.insert("tbd2", QJsonValue::fromVariant(value));

            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }
*/
    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);

            QList<QByteArray> elements = temp.split(',');

            int i=0;
            foreach(auto e, elements) {
                responseList.at(i++)->setValue(e);
            }

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

#endif // CMDQUERYfpadj_H
