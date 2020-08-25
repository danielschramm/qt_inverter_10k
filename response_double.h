#ifndef RESPONSE_DOUBLE_H
#define RESPONSE_DOUBLE_H

#include "iResponse.h"

class ResposeDouble :public iResponse {
private:
    QString jsonKey;
    qlonglong value=0;
    double divider=1.0;
public:
    ResposeDouble(const QString jsonKey, double divider) : jsonKey(jsonKey), divider(divider) {

    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong();
    }

    virtual QString getJsonKey() {
        return jsonKey;
    }

    virtual QJsonValue getJsonValue() {
        //return QJsonValue::fromVariant(static_cast<double>(value) / divider);
        return static_cast<double>(value / divider);
    }

};

#endif // RESPONSE_DOUBLE_H
