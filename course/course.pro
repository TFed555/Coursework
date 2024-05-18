QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    db.cpp \
    descwork.cpp \
    edituser.cpp \
    editworkswindow.cpp \
    main.cpp \
    autho.cpp \
    registration.cpp \
    tablemodel.cpp \
    test.cpp \
    userswindow.cpp \
    worksmodel.cpp \
    workswindow.cpp

HEADERS += \
    autho.h \
    custombox.h \
    db.h \
    descwork.h \
    edituser.h \
    editworkswindow.h \
    registration.h \
    tablemodel.h \
    test.h \
    userswindow.h \
    worksmodel.h \
    workswindow.h

FORMS += \
    autho.ui \
    descwork.ui \
    edituser.ui \
    editworkswindow.ui \
    registration.ui \
    test.ui \
    userswindow.ui \
    workswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
