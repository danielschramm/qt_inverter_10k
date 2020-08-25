#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <QString>
#include <QJsonValue>

class iResponse {
public:
    virtual void setValue(QByteArray newValue) = 0;
    virtual QString getJsonKey() = 0;
    virtual QJsonValue getJsonValue() = 0;
};


#endif // IRESPONSE_H
