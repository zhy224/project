QT       += core gui network

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

TRANSLATIONS += Resource\translation\Ch.ts \
                Resource\translation\En.ts

INCLUDEPATH += \
    LightButton/

SOURCES += \
    Command/command.cpp \
    LightButton/lightbutton.cpp \
    MsgBox/msgbox.cpp \
    Public/SC.cpp \
    Public/cn.cpp \
    main.cpp \
    mainwindow.cpp \
    widget/deploy.cpp \
    widget/proparset.cpp

HEADERS += \
    Command/command.h \
    LightButton/lightbutton.h \
    MsgBox/msgbox.h \
    Public/DT.h \
    Public/SC.h \
    Public/SD.h \
    Public/cn.h \
    mainwindow.h \
    widget/deploy.h \
    widget/proparset.h

FORMS += \
    mainwindow.ui \
    widget/deploy.ui \
    widget/proparset.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
RC_ICONS = ic_mainwindow.ico

DISTFILES += \
    Resource/translation/English.qm


