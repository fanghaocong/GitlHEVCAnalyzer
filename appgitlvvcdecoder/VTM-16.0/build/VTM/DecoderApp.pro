# ----------------------------------------------------

# ------------------------------------------------------
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt


TEMPLATE = app
TARGET = VTM_160
DESTDIR = $${OUT_PWD}/../../../../decoders
DEFINES += _CONSOLE _CRT_SECURE_NO_WARNINGS
INCLUDEPATH += ../../source/Lib
DEPENDPATH += .
include(DecoderApp.pri)

LIBS += -L$$OUT_PWD/..

CONFIG(debug, debug|release){
    LIBS += -lUtilitiesd -lDecoderLibd -lCommonLibd
}
CONFIG(release, debug|release){
    LIBS += -lUtilities -lDecoderLib -lCommonLib
}

DEPENDPATH += ../../source/Lib
message("fanghc decoderapp defines: "$$DEFINES)
