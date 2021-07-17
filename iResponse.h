#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <QString>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QRegularExpression>

class iResponse {
protected:
public:
    virtual void setValue(QByteArray newValue) = 0;
    virtual QString getJsonKey() = 0;
    virtual QJsonValue getJsonValue() = 0;
    virtual QString getAutodetectTopic() =0;
    virtual QByteArray getAutodetectPalyoad(QString stateTopic, QString availTopic, QString commandTopic) =0;
};


#endif // IRESPONSE_H
