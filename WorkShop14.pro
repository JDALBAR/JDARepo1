QT       += core gui printsupport concurrent

QMAKE_CXXFLAGS += -Wa,-mbig-obj

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = WorkShop14
TEMPLATE = app

SOURCES += \
    main.cpp \
    qcustomplot.cpp \
    workshop14.cpp

HEADERS += \
    qcustomplot.h \
    workshop14.h

FORMS += \
    workshop14.ui

CONFIG+= static

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
