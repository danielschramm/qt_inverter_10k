#ifndef ICMD_H
#define ICMD_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <crc.h>

class iCmdQuery
{
protected:

public:
    virtual QByteArray getCmd() = 0;

    virtual QByteArray getTopic() = 0;

    virtual QByteArray resultToJson(QByteArray response) = 0;

};

#endif // ICMD_H
