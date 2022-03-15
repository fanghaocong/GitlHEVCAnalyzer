HEADERS += \
    $$PWD/../../source/Lib/CommonLib/x86/AdaptiveLoopFilterX86.h \
    $$PWD/../../source/Lib/CommonLib/x86/AffineGradientSearchX86.h \
    $$PWD/../../source/Lib/CommonLib/x86/BufferX86.h \
    $$PWD/../../source/Lib/CommonLib/x86/CommonDefX86.h \
    $$PWD/../../source/Lib/CommonLib/x86/IbcHashMapX86.h \
    $$PWD/../../source/Lib/CommonLib/x86/InterpolationFilterX86.h \
    $$PWD/../../source/Lib/CommonLib/x86/RdCostX86.h

SOURCES += \
    $$PWD/../../source/Lib/CommonLib/x86/CommonDefX86.cpp \
    $$PWD/../../source/Lib/CommonLib/x86/InitX86.cpp

include(CommonLib_x86_avx.pri)
include(CommonLib_x86_avx2.pri)
include(CommonLib_x86_sse41.pri)
include(CommonLib_x86_sse42.pri)
