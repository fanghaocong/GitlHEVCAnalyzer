# ----------------------------------------------------

# ------------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    VTM/CommonLib.pro \
    VTM/DecoderLib.pro \
    VTM/Utilities.pro \
    VTM/SysuAnalyzerLib.pro \
    VTM/DecoderApp.pro

QMAKE_CXXFLAGS += "-msse4.1"
