QT += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    archiver.cpp \
    config.cpp \
    downloader.cpp \
    main.cpp \
    mainwindow.cpp \
    qregexp.cpp \
    startbutton.cpp \
    versionbutton.cpp \
    versions.cpp

HEADERS += \
    archiver.h \
    config.h \
    downloader.h \
    json.hpp \
    mainwindow.h \
    qregexp.h \
    startbutton.h \
    versionbutton.h \
    versions.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

INCLUDEPATH += "C:\Users\Den\vcpkg\packages\libssh_x64-windows\include"
LIBS += -L"C:\Users\Den\vcpkg\packages\libssh_x64-windows\lib" -lssh
INCLUDEPATH += "C:\Users\Den\vcpkg\packages\libzip_x64-windows\include"
LIBS += -L"C:\Users\Den\vcpkg\packages\libzip_x64-windows\lib" -lzip
