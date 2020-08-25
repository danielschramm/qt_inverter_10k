#ifndef CMDQUERYENERGYYEAR_H
#define CMDQUERYENERGYYEAR_H

#include <QString>
#include <QByteArray>
#include <QtDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <icmdquery.h>

class P17QueryEnergyYear : public iCmdQuery
{
private:
    bool isValidFormat(QByteArray response) {
        bool result=true;

        if(! response.endsWith("\r"))
            result=false;
        if(! response.startsWith("^D011"))
            result=false;
        Crc crc;
        if(! crc.checkResponse(response))
            result=false;

        return result;
    }

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
    P17QueryEnergyYear() {

    }

    virtual QByteArray getCmd() {
        QByteArray returnval = "^P010EY";
        QByteArray year;
        year.setNum(QDate::currentDate().year());
        returnval.append(year);

        returnval.append(genChecksum(returnval));

        returnval.append("\r");
        return(returnval);
    }

    virtual QByteArray getTopic() {
        return("/YearlyEnergy");
    }

    virtual QByteArray resultToJson(QByteArray response) {
        if(isValidFormat(response)) {
            QByteArray temp = response.right(response.size() - 5);
            temp.truncate(temp.size() - 3);
            qlonglong energy =  temp.toLongLong();
            qDebug() << "result: " << energy;


            QJsonObject  recordObject;
            recordObject.insert("YearlyEnergy", QJsonValue::fromVariant(energy));
            QJsonDocument doc(recordObject);
            return(doc.toJson());
        } else {
            qDebug() << "wrong Format";
            return("");
        }
        return("");
    }


};

#endif // CMDQUERYENERGYYEAR_H
