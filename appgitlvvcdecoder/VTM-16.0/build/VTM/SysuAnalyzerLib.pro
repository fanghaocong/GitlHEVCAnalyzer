# ----------------------------------------------------

# ------------------------------------------------------

TEMPLATE = lib
CONFIG(debug, debug|release){
    TARGET = SysuAnalyzerLibd
}
CONFIG(release, debug|release){
    TARGET = SysuAnalyzerLib
}
CONFIG += staticlib
DEFINES += _CRT_SECURE_NO_WARNINGS
INCLUDEPATH += ../../source/Lib \
    ../../../SysuAnalyzerLib
DEPENDPATH += .
DESTDIR = $${OUT_PWD}/..
include(SysuAnalyzerLib.pri)
