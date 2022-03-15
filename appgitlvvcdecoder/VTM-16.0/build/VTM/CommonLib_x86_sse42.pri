SOURCES += \
    $$PWD/../../source/Lib/CommonLib/x86/sse42/IbcHashmap_sse42.cpp

DEFINES += USE_SSE42
QMAKE_CXXFLAGS += "-msse4.2"
