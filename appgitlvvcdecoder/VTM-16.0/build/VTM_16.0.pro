# ----------------------------------------------------

# ------------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered c++11
SUBDIRS += \
    VTM/Utilities.pro \
    VTM/CommonLib.pro \
    VTM/DecoderLib.pro \
    VTM/DecoderApp.pro \

QMAKE_CXXFLAGS += "-msse4.1"
