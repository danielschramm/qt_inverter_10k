#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <Qt>
#include <QObject>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


class ConfigOptions {
public:
    QString mqttServerName="";
    int mqttServerPort=1883;
    QString mqttServerPass="";
    QString mqttServerUser="";
    QString deviceName="";
    QString protocol="";

    QString serialServerName="";
    int serialServerPort=23;

    int waittime=10000;
};



class ConfigFile
{
private:
    QString configFileName="config.json";
public:

    ConfigOptions configOptions;

    ConfigFile(QString &newConfigFileName) {
        if(newConfigFileName.size()>0) {
            configFileName=newConfigFileName;
        }
    }

    void init() {
        QFile loadFile(configFileName);

            if (!loadFile.open(QIODevice::ReadOnly)) {
                qWarning("Couldn't open config.json file.");
                return;
            }

            QByteArray configData = loadFile.readAll();
            loadFile.close();


            QJsonDocument loadDoc(QJsonDocument::fromJson(configData));

            QJsonObject jsonObj = loadDoc.object();

            QJsonDocument docdbg(jsonObj);

            configOptions.mqttServerName = jsonObj["mqttServerName"].toString();
            configOptions.mqttServerPort = jsonObj["mqttServerPort"].toInt();
            configOptions.mqttServerPass = jsonObj["mqttServerPass"].toString();
            configOptions.mqttServerUser = jsonObj["mqttServerUser"].toString();
            configOptions.deviceName = jsonObj["deviceName"].toString();
            configOptions.protocol = jsonObj["protocol"].toString();
            configOptions.serialServerName = jsonObj["serialServerName"].toString();
            configOptions.serialServerPort = jsonObj["serialServerPort"].toInt();
            configOptions.waittime = jsonObj["waittime"].toInt();



            //read(loadDoc.object());
    }

    ConfigOptions& getConfigOptions() {
        return configOptions;
    }
};

#endif // CONFIGFILE_H
