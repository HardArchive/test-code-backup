TEMPLATE     = lib
CONFIG	    += dll
SOURCES	     = main.cpp \
    kpsdk.cpp \
    publicFun.cpp

TARGET	     = kpSDK
DLLDESTDIR   = $$[QT_INSTALL_PREFIX]/bin

include(../../src/qtwinmigrate.pri)

HEADERS += \
    kpsdk.h \
    publicFun.h
QT      +=network  webkit




