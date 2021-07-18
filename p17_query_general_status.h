#ifndef CMDQUERYgs_H
#define CMDQUERYgs_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include <response_double.h>
#include <response_enum.h>
#include <response_longlong.h>


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
    P17QueryGeneralStatus(QString devName) : iCmdQuery("GeneralStatus", devName) {
        responseList.append(new ResponseDouble(devName, "solar_input_voltage_1", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "solar_input_voltage_2", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "solar_input_current_1", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "solar_input_current_2", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "battery_voltage", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "battery_capacity", 1.0, "%", "battery"));
        responseList.append(new ResponseDouble(devName, "battery_current", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "ac_input_voltage_r", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "ac_input_voltage_s", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "ac_input_voltage_t", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "ac_input_frequency", 100.0, "Hz"));
        responseList.append(new ResponseDouble(devName, "ac_input_current_r", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "ac_input_current_s", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "ac_input_current_t", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "ac_output_voltage_r", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "ac_output_voltage_s", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "ac_output_voltage_t", 10.0, "V"));
        responseList.append(new ResponseDouble(devName, "ac_output_frequency", 100.0, "Hz"));
        responseList.append(new ResponseDouble(devName, "ac_output_current_r", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "ac_output_current_s", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "ac_output_current_t", 100.0, "A"));
        responseList.append(new ResponseDouble(devName, "inner_temperature", 1.0, "°C", "temperature"));
        responseList.append(new ResponseDouble(devName, "component_max_temperature",  1.0, "°C", "temperature"));
        responseList.append(new ResponseDouble(devName, "external_battery_temperature",  1.0, "°C", "temperature"));

        QMap <int, QString> enumChanged;
        enumChanged[0]="Nothing changed";
        enumChanged[1]="Something changed";
        responseList.append(new ResponseEnum(devName, "setting_change_bit",enumChanged ));
    }

    virtual QByteArray getCmd() {
        return("^P003GS\r");
    }

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
            qDebug() << "GeneralStatus: wrong Format";
            return("");
        }
        return("");
    }
};

#endif // CMDQUERYgs_H
