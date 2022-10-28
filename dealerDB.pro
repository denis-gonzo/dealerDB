QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogcheck.cpp \
    dialogclient.cpp \
    dialogcontractsale.cpp \
    dialogcontractsupply.cpp \
    dialogpayment.cpp \
    dialogproduct.cpp \
    dialogproductsale.cpp \
    dialogproductsupply.cpp \
    dialogproducttype.cpp \
    dialogqueryclients.cpp \
    dialogtabledb.cpp \
    dialogvendor.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    customtabledelegate.h \
    dialogcheck.h \
    dialogclient.h \
    dialogcontractsale.h \
    dialogcontractsupply.h \
    dialogpayment.h \
    dialogproduct.h \
    dialogproductsale.h \
    dialogproductsupply.h \
    dialogproducttype.h \
    dialogqueryclients.h \
    dialogtabledb.h \
    dialogvendor.h \
    mainwindow.h

FORMS += \
    dialogcheck.ui \
    dialogclient.ui \
    dialogcontractsale.ui \
    dialogcontractsupply.ui \
    dialogpayment.ui \
    dialogproduct.ui \
    dialogproductsale.ui \
    dialogproductsupply.ui \
    dialogproducttype.ui \
    dialogqueryclients.ui \
    dialogtabledb.ui \
    dialogvendor.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rasources.qrc

DISTFILES += \
    init_db.sql
