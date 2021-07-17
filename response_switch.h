#ifndef RESPONSE_SWITCH_H
#define RESPONSE_SWITCH_H

#include "iResponse.h"


class ResponseSwitch :public iResponse {
private:
    QString devName;
    QString valueName;
    bool value=0;
    QString deviceClass;
public:
    ResponseSwitch(const QString devName, const QString valueName, QString deviceClass="") :
        devName(devName),
        valueName(valueName),
        deviceClass(deviceClass) {
    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong() > 0;
    }

    virtual QString getJsonKey() {
        return devName + "_" + valueName;
    }

    virtual QString getUnit() {
        return "";
    }

    virtual QJsonValue getJsonValue() {
        return static_cast<double>(value);
    }

    virtual QString getAutodetectTopic() {
        QString returnStr="homeassistant/switch/" + devName + "/" + getJsonKey() + "/config";
        returnStr.replace(QRegularExpression("\\s+"), "_");
        return returnStr;
    }

    virtual QByteArray getAutodetectPalyoad(
            QString stateTopic, QString availTopic, QString commandTopic) {
        QJsonObject  recordObject;
        recordObject.insert("name", devName + " " + valueName);
        recordObject.insert("unique_id", devName + "_" + valueName);
        recordObject.insert("state_topic", stateTopic);

        if(commandTopic.length()>0) {
            recordObject.insert("command_topic", commandTopic);
        }
        if(availTopic.length()>0) {
            recordObject.insert("availability_topic", availTopic);
        }

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

#endif // RESPONSE_SWITCH_H
