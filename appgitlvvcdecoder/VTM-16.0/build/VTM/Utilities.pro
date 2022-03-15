# ----------------------------------------------------

# ------------------------------------------------------


TEMPLATE = lib
CONFIG(debug, debug|release){
    TARGET = Utilitiesd
}
CONFIG(release, debug|release){
    TARGET = Utilities
}
CONFIG += staticlib
DEFINES += _CRT_SECURE_NO_WARNINGS
INCLUDEPATH += ../../source/Lib
DEPENDPATH += .
DESTDIR = $${OUT_PWD}/..
include(Utilities.pri)

message("Utilities -out_pwd:"$${OUT_PWD})
