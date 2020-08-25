#ifndef RESPONSE_ENUM_H
#define RESPONSE_ENUM_H

#include "iResponse.h"
#include <QMap>

class ResposeEnum :public iResponse {
private:
    QString jsonKey;
    QMap <int, QString> enumeration;
    int value=0;
public:
    ResposeEnum(const QString jsonKey, QMap <int, QString> &enumeration) : jsonKey(jsonKey), enumeration(enumeration) {

    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong();
    }

    virtual QString getJsonKey() {
        return jsonKey;
    }

    virtual QJsonValue getJsonValue() {
        return enumeration[value];
    }

};


#endif // RESPONSE_ENUM_H
