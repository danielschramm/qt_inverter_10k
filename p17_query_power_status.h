#ifndef CMDQUERYps_H
#define CMDQUERYps_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>

class P17QueryPowerStatus : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D101"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

public:
    P17QueryPowerStatus() {

    }

    virtual QByteArray getCmd() {
        return("^P003PS\r");
    }

    virtual QByteArray getTopic() {
        return("/PowerStatus");
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);

            QList<QByteArray> elements = temp.split(',');

            QJsonObject  recordObject;
            qlonglong value;

            value = elements.at(0).toLongLong();
            recordObject.insert("solar_input_power_1", QJsonValue::fromVariant(value));
            value = elements.at(1).toLongLong();
            recordObject.insert("solar_input_power_2", QJsonValue::fromVariant(value));

            value = elements.at(2).toLongLong();
            recordObject.insert("battery_power", QJsonValue::fromVariant(value));

            value = elements.at(3).toLongLong();
            recordObject.insert("ac_input_active_power_r", QJsonValue::fromVariant(value));
            value = elements.at(4).toLongLong();
            recordObject.insert("ac_input_active_power_s", QJsonValue::fromVariant(value));
            value = elements.at(5).toLongLong();
            recordObject.insert("ac_input_active_power_t", QJsonValue::fromVariant(value));
            value = elements.at(6).toLongLong();
            recordObject.insert("ac_input_total_active_power", QJsonValue::fromVariant(value));
            value = elements.at(7).toLongLong();
            recordObject.insert("ac_output_active_power_r", QJsonValue::fromVariant(value));
            value = elements.at(8).toLongLong();
            recordObject.insert("ac_output_active_power_s", QJsonValue::fromVariant(value));
            value = elements.at(9).toLongLong();
            recordObject.insert("ac_output_active_power_t", QJsonValue::fromVariant(value));
            value = elements.at(10).toLongLong();
            recordObject.insert("ac_output_total_active_power", QJsonValue::fromVariant(value));
            value = elements.at(11).toLongLong();
            recordObject.insert("ac_output_apperent_power_r", QJsonValue::fromVariant(value));
            value = elements.at(12).toLongLong();
            recordObject.insert("ac_output_apperent_power_s", QJsonValue::fromVariant(value));
            value = elements.at(13).toLongLong();
            recordObject.insert("ac_output_apperent_power_t", QJsonValue::fromVariant(value));
            value = elements.at(14).toLongLong();
            recordObject.insert("ac_output_total_apperent_power", QJsonValue::fromVariant(value));
            value = elements.at(15).toLongLong();
            recordObject.insert("ac_output_power_percentage", QJsonValue::fromVariant(value));
            value = elements.at(16).toLongLong();
            recordObject.insert("ac_output_connect_status", QJsonValue::fromVariant(value));
            value = elements.at(17).toLongLong();
            recordObject.insert("solar_input_1_work_status", QJsonValue::fromVariant(value));
            value = elements.at(18).toLongLong();
            recordObject.insert("solar_input_2_work_status", QJsonValue::fromVariant(value));
            value = elements.at(19).toLongLong();
            recordObject.insert("battery_power_direction", QJsonValue::fromVariant(value));
            value = elements.at(20).toLongLong();
            recordObject.insert("dc_ac_power_direction", QJsonValue::fromVariant(value));
            value = elements.at(21).toLongLong();
            recordObject.insert("line_power_direction", QJsonValue::fromVariant(value));


            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};

#endif // CMDQUERYps_H
