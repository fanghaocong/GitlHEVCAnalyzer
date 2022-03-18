# ----------------------------------------------------

# ------------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered c++11
SUBDIRS += \
    VTM/CommonLib.pro \
    VTM/DecoderLib.pro \
    VTM/Utilities.pro \
    VTM/DecoderApp.pro \

QMAKE_CXXFLAGS += "-msse4.1"
