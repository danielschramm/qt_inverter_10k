#ifndef P18QUERYgs_H
#define P18QUERYgs_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include "crc.h"


class P18QueryGeneralStatus : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D106"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

public:
    P18QueryGeneralStatus() {

    }

    virtual QByteArray getCmd() {
        QByteArray cmd="^P005GS";
        Crc crc;
        crc.appendCrc(cmd);
        cmd.append('\r');
        qDebug() << cmd;
        return(cmd);
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
            recordObject.insert("grid_voltage", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(1).toLongLong();
            recordObject.insert("grid_frequency", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(2).toLongLong();
            recordObject.insert("ac_output_voltage", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(3).toLongLong();
            recordObject.insert("ac_output_frequency", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(4).toLongLong();
            recordObject.insert("ac_output_apparent_power", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(5).toLongLong();
            recordObject.insert("ac_output_active_power", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(6).toLongLong();
            recordObject.insert("output_load_percent", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(7).toLongLong();
            recordObject.insert("battery_voltage", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(8).toLongLong();
            recordObject.insert("battery_voltage_scc", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(9).toLongLong();
            recordObject.insert("battery_voltage_scc2", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(10).toLongLong();
            recordObject.insert("battery_discharge_current", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(11).toLongLong();
            recordObject.insert("battery_charge_current", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(12).toLongLong();
            recordObject.insert("battery_capacity", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(13).toLongLong();
            recordObject.insert("inverter_heatsink_temperature", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(14).toLongLong();
            recordObject.insert("inverter_mppt1_temperature", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(15).toLongLong();
            recordObject.insert("inverter_mppt2_temperature", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(16).toLongLong();
            recordObject.insert("pv1_input_power", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(17).toLongLong();
            recordObject.insert("pv2_input_power", QJsonValue::fromVariant(static_cast<double>(value) / 1.0));
            value = elements.at(18).toLongLong();
            recordObject.insert("pv1_input_voltage", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(19).toLongLong();
            recordObject.insert("pv2_input_voltage", QJsonValue::fromVariant(static_cast<double>(value) / 10.0));
            value = elements.at(20).toLongLong();
            recordObject.insert("settings_changed", QJsonValue::fromVariant(value));
            value = elements.at(21).toLongLong();
            recordObject.insert("mppt1_charger_status", QJsonValue::fromVariant(value));
            value = elements.at(22).toLongLong();
            recordObject.insert("mppt2_charger_status", QJsonValue::fromVariant(value));
            value = elements.at(23).toLongLong();
            recordObject.insert("load_connection", QJsonValue::fromVariant(value));
            value = elements.at(24).toLongLong();
            recordObject.insert("battery_power_direction", QJsonValue::fromVariant(value));
            value = elements.at(25).toLongLong();
            recordObject.insert("dc_ac_power_direction", QJsonValue::fromVariant(value));
            value = elements.at(26).toLongLong();
            recordObject.insert("line_power_direction", QJsonValue::fromVariant(value));
            value = elements.at(27).toLongLong();
            recordObject.insert("local_parallel_id", QJsonValue::fromVariant(value));


            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};

#endif // P18QUERYgs_H
