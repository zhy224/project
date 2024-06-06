QT       += core gui network axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    Command/command.cpp \
    MsgBox/msgbox.cpp \
    Public/SC.cpp \
    Public/cn.cpp \
    QExcel/qexcel.cpp \
    SubWidget/mcb_test.cpp \
    SubWidget/networkconnect.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Command/command.h \
    MsgBox/msgbox.h \
    Public/DT.h \
    Public/SC.h \
    Public/cn.h \
    QExcel/qexcel.h \
    SubWidget/mcb_test.h \
    SubWidget/networkconnect.h \
    mainwindow.h

FORMS += \
    SubWidget/mcb_test.ui \
    SubWidget/networkconnect.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
