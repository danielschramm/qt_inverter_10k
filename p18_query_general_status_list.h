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
    QList<iResponse *> responseList;

public:
    P18QueryGeneralStatus() {
        responseList.append(new ResposeDouble("grid_voltage",  10.0));
        responseList.append(new ResposeDouble("grid_frequency",  10.0));
        responseList.append(new ResposeDouble("ac_output_voltage",  10.0));
        responseList.append(new ResposeDouble("ac_output_frequency",  10.0));
        responseList.append(new ResposeDouble("ac_output_apparent_power",  10.0));
        responseList.append(new ResposeDouble("ac_output_active_power",  10.0));
        responseList.append(new ResposeDouble("output_load_percent",  1.0));
        responseList.append(new ResposeDouble("battery_voltage",  10.0));
        responseList.append(new ResposeDouble("battery_voltage_scc",  10.0));
        responseList.append(new ResposeDouble("battery_voltage_scc2",  10.0));
        responseList.append(new ResposeDouble("battery_discharge_current",  1.0));
        responseList.append(new ResposeDouble("battery_charge_current",  1.0));
        responseList.append(new ResposeDouble("battery_capacity",  1.0));
        responseList.append(new ResposeDouble("inverter_heatsink_temperature",  1.0));
        responseList.append(new ResposeDouble("inverter_mppt1_temperature",  1.0));
        responseList.append(new ResposeDouble("inverter_mppt2_temperature",  1.0));
        responseList.append(new ResposeDouble("pv1_input_power",  1.0));
        responseList.append(new ResposeDouble("pv2_input_power",  1.0));
        responseList.append(new ResposeDouble("pv1_input_voltage",  10.0));
        responseList.append(new ResposeDouble("pv2_input_voltage",  10.0));

        QMap <int, QString> enumChanged;
        enumChanged[0]="Nothing changed";
        enumChanged[1]="Something changed";
        responseList.append(new ResposeEnum("settings_changed",enumChanged ));

        QMap <int, QString> enumMppt;
        enumMppt[0]="abnormal";
        enumMppt[1]="normal but not charged";
        enumMppt[2]="charging";
        responseList.append(new ResposeEnum("mppt1_charger_status",enumMppt ));
        responseList.append(new ResposeEnum("mppt2_charger_status",enumMppt ));

        QMap <int, QString> enumLoad;
        enumLoad[0]="disconnected";
        enumLoad[1]="connected";
        responseList.append(new ResposeEnum("load_connection", enumLoad ));

        QMap <int, QString> enumBatt;
        enumBatt[0]="donothing";
        enumBatt[1]="charge";
        enumBatt[2]="discharge";
        responseList.append(new ResposeEnum("battery_power_direction",enumBatt ));

        QMap <int, QString> enumDCAC;
        enumDCAC[0]="donothing";
        enumDCAC[1]="AC-DC";
        enumDCAC[2]="DC-AC";
        responseList.append(new ResposeEnum("dc_ac_power_direction" ,enumDCAC));

        QMap <int, QString> enumLine;
        enumDCAC[0]="donothing";
        enumDCAC[1]="input";
        enumDCAC[2]="output";
        responseList.append(new ResposeEnum("line_power_direction", enumLine ));
        responseList.append(new ResposeLongLong("local_parallel_id" ));
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
