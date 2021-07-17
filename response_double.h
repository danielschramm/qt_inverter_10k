#ifndef RESPONSE_DOUBLE_H
#define RESPONSE_DOUBLE_H

#include "iResponse.h"


class ResponseDouble :public iResponse {
private:
    QString devName;
    QString valueName;
    qlonglong value=0;
    double divider=1.0;
    QString unit;
    QString deviceClass;
public:
    ResponseDouble(const QString devName, const QString valueName, double divider, QString unit, QString deviceClass="") :
        devName(devName),
        valueName(valueName),
        divider(divider),
        unit(unit),
        deviceClass(deviceClass) {
    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong();
    }

    virtual QString getJsonKey() {
        return devName + "_" + valueName;
    }

    virtual QString getUnit() {
        return unit;
    }

    virtual QJsonValue getJsonValue() {
        return static_cast<double>(value / divider);
    }

    virtual QString getAutodetectTopic() {
        QString returnStr="homeassistant/sensor/" + devName + "/" + getJsonKey() + "/config";
        returnStr.replace(QRegularExpression("\\s+"), "_");
        return returnStr;
    }

    virtual QByteArray getAutodetectPalyoad(
            QString stateTopic, QString availTopic, QString commandTopic) {
        QJsonObject  recordObject;
        recordObject.insert("unit_of_measurement", unit);
        recordObject.insert("name", devName + " " + valueName);
        recordObject.insert("unique_id", devName + "_" + valueName);
        recordObject.insert("state_topic", stateTopic);
        if(availTopic.length()>0) {
            recordObject.insert("availability_topic", availTopic);
        }

        QString valueTemplate="{{ value_json.";
        valueTemplate.append(getJsonKey());
        valueTemplate.append("}}");
        recordObject.insert("value_template", valueTemplate);

        if(deviceClass.length()>0) {
            recordObject.insert("device_class", deviceClass);
        }

        QJsonObject deviceObject;
        deviceObject.insert("name", devName);
        deviceObject.insert("identifiers", devName);
        deviceObject.insert("sw_version", "0.0");
        deviceObject.insert("model", "Infini Solar");
        deviceObject.insert("manufacturer", "S/W Daniel Schramm");
        recordObject.insert("device", deviceObject);

        QJsonDocument doc(recordObject);
        return(doc.toJson());
    }

};

#endif // RESPONSE_DOUBLE_H
