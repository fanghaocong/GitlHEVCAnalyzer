# ----------------------------------------------------

# ------------------------------------------------------

TEMPLATE = lib
CONFIG(debug, debug|release){
    TARGET = DecoderLibd
}
CONFIG(release, debug|release){
    TARGET = DecoderLib
}
CONFIG += staticlib
DEFINES += _CRT_SECURE_NO_WARNINGS
INCLUDEPATH += ../../source/Lib \
    ../../..
DEPENDPATH += .
DESTDIR = $${OUT_PWD}/..
include(DecoderLib.pri)
