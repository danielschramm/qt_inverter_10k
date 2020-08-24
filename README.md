# qt_inverter_10k

QT Software to read data from Infini Solar 10kW 3 phase inverters with P17 protocol and some 1 phase interters using P18 protocol.

The inverter needs to be connected to a RS232<->Network converter to use this project.
(Alternative a Serial to Network link can be setup under Linux with netcat, etc)
Data is published as json objects to a MQTT Server.

Every Inverter query is mapped to one mqtt message and new queries can easily be added by deriving a new class from iCmdQuery.

Currently the following P17 commands are implemented:
- ^P007EMINFO (Feedin Power)
- ^P003ET (Total Energy generated)
- ^P003PS (Power Status)
- ^P003GS (General Status)

Currently the following P18 commands are implemented:
- ^P005GS (General Status)
- ^P005ET (Total Energy generated)
- ^P009EY (Yearly Energy generated)
- ^P011EM (Monthly Energy generated)

Commands to the inverter are currently not scope of the project but pull requests including this are highly welcome.

For the mqtt connection the qmqtt library has to be manually added to the QT community edition.

The code runs from console and does not use any platform specific code so it should work on every platform supported by QT.

Edit config.json to match your local network settings and copy it to the same folder containing the executeable. 

Settings for the Serial to Network converter:
- TCP mode
- 2400 Baud 8N1
- Delimiter -> Silent time 1s (5*200ms)
The delimiter is important for the software to read the answer from the inverter.
