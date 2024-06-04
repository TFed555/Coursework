QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
RESOURCES += new.qrc
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractwork.cpp \
    adminwindow.cpp \
    creatework.cpp \
    db.cpp \
    descwork.cpp \
    edituser.cpp \
    editwork.cpp \
    editworkswindow.cpp \
    main.cpp \
    autho.cpp \
    organiserwindow.cpp \
    qdynamicedit.cpp \
    registration.cpp \
    usersmodel.cpp \
    userswindow.cpp \
    workerwindow.cpp \
    worksmodel.cpp \
    workswindow.cpp

HEADERS += \
    abstractwork.h \
    adminwindow.h \
    autho.h \
    creatework.h \
    custombox.h \
    db.h \
    descwork.h \
    editsproxymodel.h \
    edituser.h \
    editwork.h \
    editworkswindow.h \
    organiserwindow.h \
    qdynamicedit.h \
    registration.h \
    usersmodel.h \
    userswindow.h \
    workerwindow.h \
    worksmodel.h \
    worksproxymodel.h \
    workswindow.h

FORMS += \
    adminwindow.ui \
    autho.ui \
    creatework.ui \
    descwork.ui \
    edituser.ui \
    editwork.ui \
    editworkswindow.ui \
    organiserwindow.ui \
    registration.ui \
    userswindow.ui \
    workerwindow.ui \
    workswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
