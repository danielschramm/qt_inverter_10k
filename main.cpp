#include <QCoreApplication>
#include <QtCore>
#include <QObject>
#include <QByteArray>


#include "maininverter.h"
#include "tcpread.h"
#include "cmdtask.h"
#include "configfile.h"



int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    ConfigFile *configFile = new ConfigFile();
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

    int result = a.exec();

    qDebug() << "closing app: " << result;

    return result;
}
