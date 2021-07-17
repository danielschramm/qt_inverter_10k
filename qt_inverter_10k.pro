QT -= gui
QT += mqtt
QT += core
QT += network


CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cmdtask.cpp \
        main.cpp \
        maininverter.cpp \
        tcpread.cpp

TRANSLATIONS += \
    qt_inverter_10k_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cmdtask.h \
    configfile.h \
    crc.h \
    iResponse.h \
    icmdquery.h \
    maininverter.h \
    p17_cmd_battery_self_test.h \
    p17_query_eminfo.h \
    p17_query_energy_year.h \
    p17_query_general_status.h \
    p17_query_power_status.h \
    p17_query_total_energy.h \
    p18_query_energy_month.h \
    p18_query_energy_year.h \
    p18_query_general_status_list.h \
    p18_query_total_energy.h \
    p18_query_working_mode.h \
    response_double.h \
    response_enum.h \
    response_longlong.h \
    response_switch.h \
    tcpread.h

DISTFILES += \
    README.md \
    config.json \
    config_privat.json
