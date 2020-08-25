#ifndef CMDQUERYgs_H
#define CMDQUERYgs_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>


class P17QueryGeneralStatus : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D110"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

public:
    P17QueryGeneralStatus() {

    }

    virtual QByteArray getCmd() {
        return("^P003GS\r");
    }

    virtual QByteArray getTopic() {
        return("/GeneralStatus");
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);

            QList<QByteArray> elements = temp.split(',');

            QJsonObject  recordObject;
            qlonglong value;

            value = elements.at(0).toLongLong();
            recordObject.insert("solar_input_voltage_1", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(1).toLongLong();
            recordObject.insert("solar_input_voltage_2", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(2).toLongLong();
            recordObject.insert("solar_input_current_1", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(3).toLongLong();
            recordObject.insert("solar_input_current_2", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(4).toLongLong();
            recordObject.insert("battery_voltage", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(5).toLongLong();
            recordObject.insert("battery_capacity", QJsonValue::fromVariant(value));
            value = elements.at(6).toLongLong();
            recordObject.insert("battery_current", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(7).toLongLong();
            recordObject.insert("ac_input_voltage_r", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(8).toLongLong();
            recordObject.insert("ac_input_voltage_s", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(9).toLongLong();
            recordObject.insert("ac_input_voltage_t", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(10).toLongLong();
            recordObject.insert("ac_input_frequency", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(11).toLongLong();
            recordObject.insert("ac_input_current_r", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(12).toLongLong();
            recordObject.insert("ac_input_current_s", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(13).toLongLong();
            recordObject.insert("ac_input_current_t", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(14).toLongLong();
            recordObject.insert("ac_output_voltage_r", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(15).toLongLong();
            recordObject.insert("ac_output_voltage_s", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(16).toLongLong();
            recordObject.insert("ac_output_voltage_t", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(17).toLongLong();
            recordObject.insert("ac_output_frequency", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(18).toLongLong();
            recordObject.insert("ac_output_current_r", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(19).toLongLong();
            recordObject.insert("ac_output_current_s", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(20).toLongLong();
            recordObject.insert("ac_output_current_t", QJsonValue::fromVariant(static_cast<double>(value) / 100.0));
            value = elements.at(21).toLongLong();
            recordObject.insert("inner_temperature", QJsonValue::fromVariant(value));
            value = elements.at(22).toLongLong();
            recordObject.insert("component_max_temperature", QJsonValue::fromVariant(value));
            value = elements.at(23).toLongLong();
            recordObject.insert("external_battery_temperature", QJsonValue::fromVariant(value));
            value = elements.at(24).toLongLong();
            recordObject.insert("setting_change_bit", QJsonValue::fromVariant(value));


            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};

#endif // CMDQUERYgs_H
