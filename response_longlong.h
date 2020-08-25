#ifndef RESPOSE_LONGLONG_H
#define RESPOSE_LONGLONG_H

#include "iResponse.h"

class ResposeLongLong :public iResponse {
private:
    QString jsonKey;
    qlonglong value=0;
public:
    ResposeLongLong(const QString jsonKey) : jsonKey(jsonKey) {

    }

    virtual void setValue(QByteArray newValue) {
        value = newValue.toLongLong();
    }

    virtual QString getJsonKey() {
        return jsonKey;
    }

    virtual QJsonValue getJsonValue() {
        return QJsonValue::fromVariant(value);
    }

};


#endif // RESPOSE_LONGLONG_H
