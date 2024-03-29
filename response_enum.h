#ifndef RESPONSE_ENUM_H
#define RESPONSE_ENUM_H

#include "iResponse.h"
#include <QMap>

class ResponseEnum :public iResponse {
private:
    QString devName;
    QString valueName;
    QMap <int, QString> enumeration;
    int value=0;
public:
    ResponseEnum(const QString devName, const QString valueName, QMap <int, QString> &enumeration) :
        devName(devName),
        valueName(valueName),
        enumeration(enumeration) {

    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong();
    }

    virtual QString getJsonKey() {
        return devName + "_" + valueName;
    }

    virtual QJsonValue getJsonValue() {
        return enumeration[value];
    }

    virtual QString getAutodetectTopic() {
        QString returnStr="homeassistant/sensor/" + devName + "/" + getJsonKey() + "/config";
        returnStr.replace(QRegularExpression("\\s+"), "_");
        return returnStr;
    }

    virtual QByteArray getAutodetectPalyoad(QString stateTopic, QString availTopic, QString commandTopic) {
        QJsonObject  recordObject;
        recordObject.insert("name",  devName + " " + valueName);
        recordObject.insert("unique_id", devName + "_" + valueName);
        recordObject.insert("state_topic", stateTopic);
        if(availTopic.length()>0) {
            recordObject.insert("availability_topic", availTopic);
        }

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


#endif // RESPONSE_ENUM_H
