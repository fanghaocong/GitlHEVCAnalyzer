# ----------------------------------------------------

# ------------------------------------------------------


TEMPLATE = lib
CONFIG(debug, debug|release){
    TARGET = CommonLibd
}
CONFIG(release, debug|release){
    TARGET = CommonLib
}
CONFIG += staticlib
DEFINES += _CRT_SECURE_NO_WARNINGS
INCLUDEPATH += ../../source/Lib
DEPENDPATH += .
DESTDIR = $${OUT_PWD}/..
include(CommonLib.pri)
include(libmd5.pri)
