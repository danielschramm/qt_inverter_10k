# qt_inverter_10k

QT Software to read data from Infini Solar 10kW 3 phase inverters.

The inverter needs to be connected to a RS232<->Network converter to use this project.
(Alternative a Serial to Network link can be setup under Linux with netcat, etc)
Data is published as json objects to a MQTT Server.

For the mqtt connection the qmqtt library has to be manually added to the QT community edition.

The code rund from console and does not use any platform specific code so it should work on every platform supported by QT.
