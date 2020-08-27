#ifndef P18_QUERY_GENERAL_STATUS_LIST_H
#define P18_QUERY_GENERAL_STATUS_LIST_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>
#include "crc.h"

#include "response_double.h"
#include "response_enum.h"
#include "response_longlong.h"

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

    P18QueryGeneralStatus(QString devName) : iCmdQuery("GeneralStatus", devName) {
        responseList.append(new ResposeDouble(devName, "grid_voltage",  10.0, "V"));
        responseList.append(new ResposeDouble(devName, "grid_frequency",  10.0, "Hz"));
        responseList.append(new ResposeDouble(devName, "ac_output_voltage",  10.0, "V"));
        responseList.append(new ResposeDouble(devName, "ac_output_frequency",  10.0, "Hz"));
        responseList.append(new ResposeDouble(devName, "ac_output_apparent_power",  10.0, "W", "power"));
        responseList.append(new ResposeDouble(devName, "ac_output_active_power",  10.0, "W", "power"));
        responseList.append(new ResposeDouble(devName, "output_load_percent",  1.0, "%"));
        responseList.append(new ResposeDouble(devName, "battery_voltage",  10.0, "V"));
        responseList.append(new ResposeDouble(devName, "battery_voltage_scc",  10.0, "V"));
        responseList.append(new ResposeDouble(devName, "battery_voltage_scc2",  10.0, "V"));
        responseList.append(new ResposeDouble(devName, "battery_discharge_current",  1.0, "A"));
        responseList.append(new ResposeDouble(devName, "battery_charge_current",  1.0, "A"));
        responseList.append(new ResposeDouble(devName, "battery_capacity",  1.0, "%", "battery"));
        responseList.append(new ResposeDouble(devName, "inverter_heatsink_temperature",  1.0, "°C", "temperature"));
        responseList.append(new ResposeDouble(devName, "inverter_mppt1_temperature",  1.0, "°C", "temperature"));
        responseList.append(new ResposeDouble(devName, "inverter_mppt2_temperature",  1.0, "°C", "temperature"));
        responseList.append(new ResposeDouble(devName, "pv1_input_power",  1.0, "W", "power"));
        responseList.append(new ResposeDouble(devName, "pv2_input_power",  1.0, "W", "power"));
        responseList.append(new ResposeDouble(devName, "pv1_input_voltage",  10.0, "V"));
        responseList.append(new ResposeDouble(devName, "pv2_input_voltage",  10.0, "V"));

        QMap <int, QString> enumChanged;
        enumChanged[0]="Nothing changed";
        enumChanged[1]="Something changed";
        responseList.append(new ResposeEnum(devName, "settings_changed",enumChanged ));

        QMap <int, QString> enumMppt;
        enumMppt[0]="abnormal";
        enumMppt[1]="normal but not charged";
        enumMppt[2]="charging";
        responseList.append(new ResposeEnum(devName, "mppt1_charger_status",enumMppt ));
        responseList.append(new ResposeEnum(devName, "mppt2_charger_status",enumMppt ));

        QMap <int, QString> enumLoad;
        enumLoad[0]="disconnected";
        enumLoad[1]="connected";
        responseList.append(new ResposeEnum(devName, "load_connection", enumLoad ));

        QMap <int, QString> enumBatt;
        enumBatt[0]="donothing";
        enumBatt[1]="charge";
        enumBatt[2]="discharge";
        responseList.append(new ResposeEnum(devName, "battery_power_direction",enumBatt ));

        QMap <int, QString> enumDCAC;
        enumDCAC[0]="donothing";
        enumDCAC[1]="AC-DC";
        enumDCAC[2]="DC-AC";
        responseList.append(new ResposeEnum(devName, "dc_ac_power_direction" ,enumDCAC));

        QMap <int, QString> enumLine;
        enumLine[0]="donothing";
        enumLine[1]="input";
        enumLine[2]="output";
        responseList.append(new ResposeEnum(devName, "line_power_direction", enumLine ));
        responseList.append(new ResposeLongLong(devName, "local_parallel_id" ,""));
    }

    virtual QByteArray getCmd() {
        QByteArray cmd="^P005GS";
        Crc crc;
        crc.appendCrc(cmd);
        cmd.append('\r');
        qDebug() << cmd;
        return(cmd);
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
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }
};


#endif // P18_QUERY_GENERAL_STATUS_LIST_H
