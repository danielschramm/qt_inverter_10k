#include "cmdtask.h"

CmdTask::CmdTask(QObject *parent) : QObject(parent)
{
    cmdList.append(new CmdQueryTotalEnergy);
    cmdList.append(new CmdQueryEnergyYear);
    cmdList.append(new CmdQueryPowerStatus);
    cmdList.append(new CmdQueryGeneralStatus);
    cmdList.append(new CmdQueryEmInfo);


}
