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

    QString genChecksum(QByteArray data) {
        unsigned int sum=0;
        for(int i=0; i<data.size(); i++) {
            sum += data[i];
        }
        sum &= 0xff;

        QString checksum;
        checksum = QString::number(sum).rightJustified(3, '0');
        return checksum;
    }



public:
    virtual QByteArray getCmd() = 0;

    virtual QByteArray getTopic() = 0;

    virtual QByteArray resultToJson(QByteArray response) = 0;

};

#endif // ICMD_H
