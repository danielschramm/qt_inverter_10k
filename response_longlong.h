#ifndef RESPOSE_LONGLONG_H
#define RESPOSE_LONGLONG_H

#include "iResponse.h"

class ResposeLongLong :public iResponse {
private:
    QString devName;
    QString valueName;
    qlonglong value=0;
    QString unit;
public:
    ResposeLongLong(const QString devName, const QString valueName, QString unit) :
        devName(devName),
        valueName(valueName),
        unit(unit) {

    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong();
    }

    virtual QString getJsonKey() {
        return devName + "_" + valueName;
    }

    virtual QJsonValue getJsonValue() {
        return value;
    }

    virtual QString getAutodetectTopic() {
        QString returnStr="homeassistant/sensor/" + devName + "/" + getJsonKey() + "/config";
        returnStr.replace(QRegularExpression("\\s+"), "_");
        return returnStr;
    }

    virtual QByteArray getAutodetectPalyoad(QString stateTopic) {
        QJsonObject  recordObject;
        recordObject.insert("unit_of_measurement", unit);
        recordObject.insert("name",  devName + " " + valueName);
        recordObject.insert("unique_id", devName + "_" + valueName);
        recordObject.insert("state_topic", stateTopic);
        QString valueTemplate="{{ value_json.";
        valueTemplate.append(getJsonKey());
        valueTemplate.append("}}");
        recordObject.insert("value_template", valueTemplate);

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


#endif // RESPOSE_LONGLONG_H
