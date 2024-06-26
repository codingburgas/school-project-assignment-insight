QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databasemanager.cpp \
    login.cpp \
    main.cpp \
    mainmenu.cpp \
    mainwindow.cpp \
    register.cpp \
    requestaccess.cpp \
    sha256.cpp

HEADERS += \
    databasemanager.hpp \
    login.hpp \
    mainmenu.hpp \
    mainwindow.hpp \
    register.hpp \
    requestaccess.hpp \
    sha256.hpp

FORMS += \
    login.ui \
    mainmenu.ui \
    mainwindow.ui \
    register.ui \
    requestaccess.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/resources.qrc
