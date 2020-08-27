#include "/usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h"
#include <QCoreApplication>
#include <QtCore>
#include <QObject>
#include <QByteArray>
#include <QCommandLineParser>
#include <QCommandLineOption>


#include "maininverter.h"
#include "tcpread.h"
#include "cmdtask.h"
#include "configfile.h"



int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("qt_inverter_10k");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("qt_inverter_10k");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption configFileOption(QStringList() << "c" << "config",
            QCoreApplication::translate("main", "Specify config file name."),
            QCoreApplication::translate("main", "directory"));
    parser.addOption(configFileOption);

     parser.process(a);

    QString configFileName = parser.value(configFileOption);

    ConfigFile *configFile = new ConfigFile(configFileName);
    configFile->init();

    MainInverter *mainInverter = new MainInverter(&a);
    mainInverter->init(configFile->getConfigOptions());

    CmdTask *cmdTask = new CmdTask();
    cmdTask->init(configFile->getConfigOptions());


    if(! QObject::connect(
                cmdTask, SIGNAL(sReceivedData(const QByteArray &, const QByteArray &)),
                mainInverter, SLOT(onReceivedData(const QByteArray &, const QByteArray &)))) {
        qDebug() << "connect failed";
        while(1);
    }


    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(mainInverter, SIGNAL(finished()), &a, SLOT(quit()));
    QObject::connect(mainInverter, SIGNAL(mqttConnected()), cmdTask, SLOT(onMqttConnect()));

    int result = a.exec();

    qDebug() << "closing app: " << result;

    return result;
}
