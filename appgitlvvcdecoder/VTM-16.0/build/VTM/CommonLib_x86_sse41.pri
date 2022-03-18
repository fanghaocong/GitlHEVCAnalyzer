SOURCES += \
    $$PWD/../../source/Lib/CommonLib/x86/sse41/AdaptiveLoopFilter_sse41.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/sse41/AffineGradientSearch_sse41.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/sse41/Buffer_sse41.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/sse41/InterpolationFilter_sse41.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/sse41/RdCost_sse41.cpp

QMAKE_CXXFLAGS += "-msse4.1"
