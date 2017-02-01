TEMPLATE = app
TARGET = Datareduction

QT = core gui
INCLUDEPATH +=

greaterThan(QT_MAJOR_VERSION, 4):QT += widgets printsupport

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    qcustomplot.h

RESOURCES += \
    resources.qrc

