################################################################
# Veelinx
# Copyright (C) 2017   Michael W Hoag
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Veelinx License, Version 1.0
################################################################

include( veelinxconfig.pri )

QT += widgets serialport

TARGET = Veelinx
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    console.cpp \
    parser.cpp \
    splash.cpp \
    dataplot.cpp \
    inst_data.cpp \
    control_dialog.cpp


HEADERS += \
    mainwindow.h \
    console.h \
    parser.h \
    splash.h \
    dataplot.h \
    inst_data.h \
    control_dialog.h


FORMS += \
    mainwindow.ui \
    splash.ui \
    control_dialog.ui

RESOURCES += \
    terminal.qrc

TRANSLATIONS += Internationalization_de.ts \
               Internationalization_es.ts
CONFIG += \
    C++11 \
    C++14

win32{
RC_ICONS += ../icon/ndt_veelinx.ico
VERSION = $$(VEE_VERSION)
QMAKE_TARGET_COMPANY = James Instruments Inc.
QMAKE_TARGET_PRODUCT = Veelinx
QMAKE_TARGET_DESCRIPTION = Upload Software for James Instruments V-Meter
QMAKE_TARGET_COPYRIGHT = @2017 James Instruments Inc.
}

#win64{
#RC_ICONS += ../icon/ndt_veelinx.ico
#VERSION = 3.0.0.0
#QMAKE_TARGET_COMPANY = James Instruments Inc.
#QMAKE_TARGET_PRODUCT = Veelinx
#QMAKE_TARGET_DESCRIPTION = Upload Software for James Instruments V-Meter
#QMAKE_TARGET_COPYRIGHT = @2017 James Instruments Inc.
#message(" WIN64 Compile ")
#}

greaterThan(QT_MAJOR_VERSION, 4) {
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../MinGW/lib/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../MinGW/lib/ -lqwtd

INCLUDEPATH += $$PWD/../MinGW/include
DEPENDPATH += $$PWD/../MinGW/include

#contains(TARGET_ARCH, x86_64) {
#    ARCHITECTURE = x64
#    message(" WIN64 Compile ")
#    win32:CONFIG(release, debug|release): LIBS += -LC:\qwt-6.1.3-MSVC64\lib -lqwt
#    else:win32:CONFIG(debug, debug|release): LIBS += -LC:\qwt-6.1.3-MSVC64\lib -lqwtd

#    INCLUDEPATH += C:\qwt-6.1.3-MSVC64\include
#    DEPENDPATH += C:\qwt-6.1.3-MSVC64\include

#    message( $$LIBS )
#    message( $$INCLUDEPATH )
#} else {
#    ARCHITECTURE = x86
#    message(" WIN32 Compile ")
#    win32:CONFIG(release, debug|release): LIBS += -LC:\qwt-6.1.3-MSVC32\lib -lqwt
#    else:win32:CONFIG(debug, debug|release): LIBS += -LC:\qwt-6.1.3-MSVC32\lib -lqwtd

#    INCLUDEPATH += C:\qwt-6.1.3-MSVC32\include
#    DEPENDPATH += C:\qwt-6.1.3-MSVC32\include
#    message( $$LIBS )
#    message( $$INCLUDEPATH)
#}
