SOURCES += \
    $$PWD/../../source/Lib/CommonLib/x86/avx2/AdaptiveLoopFilter_avx2.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx2/AffineGradientSearch_avx2.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx2/Buffer_avx2.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx2/InterpolationFilter_avx2.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/avx2/RdCost_avx2.cpp

QMAKE_CXXFLAGS += "-mavx2"
msvc:QMAKE_CXXFLAGS += "/arch:AVX2"
