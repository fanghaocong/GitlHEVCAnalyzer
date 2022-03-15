SOURCES += \
    $$PWD/../../source/Lib/CommonLib/x86/avx/AdaptiveLoopFilter_avx.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx/AffineGradientSearch_avx.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx/Buffer_avx.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx/InterpolationFilter_avx.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx/RdCost_avx.cpp

DEFINES += USE_AVX
QMAKE_CXXFLAGS += "-mavx"
