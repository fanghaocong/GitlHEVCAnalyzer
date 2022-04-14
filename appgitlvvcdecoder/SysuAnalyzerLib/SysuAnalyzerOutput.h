#ifndef SYSUANALYZEROUTPUT_H
#define SYSUANALYZEROUTPUT_H

#include <fstream>
#include <string>
#include <vector>

#include "CommonLib/CodingStructure.h"

class SysuAnalyzerOutput
{
public:
    SysuAnalyzerOutput();
    ~SysuAnalyzerOutput();

    /// Sequence parameter set output
    void writeOutSps    ( SPS* sps );

    void writeOutCTUInfo ( CodingStructure& cs, const UnitArea& ctuArea );

    /// SINGLETON
    static SysuAnalyzerOutput* getInstance();

private:
    /// Decoder output ( extracted from bitstream )
    std::ofstream m_cSpsOut;          ///< SPS info
    std::ofstream m_cCUPUOutput;      ///< CU/PU info output
    std::ofstream m_cTUOutput;        ///< TU info output
    std::ofstream m_cPredOutput;      ///< Prediction mode info output
    std::ofstream m_cMVOutput;        ///< MV info output
    std::ofstream m_cMergeOutput;     ///< Merge info output
    std::ofstream m_cIntraOutput;     ///< Intra info output
//    std::ofstream m_cLCUBitOutput;    ///< LCU bit consumption info output
//    std::ofstream m_cSCUBitOutput;    ///< SCU bit consumption info output

    void xWriteOutCUInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
    void xWriteOutTUInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
    void xWriteOutPredInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
    void xWriteOutMVInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
    void xWriteOutIntraInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
    void xWriteOutMergeInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
//    void xWriteOutLCUBitInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );
//    void xWriteOutSCUBitInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle=CHANNEL_TYPE_LUMA );

    static SysuAnalyzerOutput* m_instance;
};

#endif // SYSUANALYZEROUTPUT_H
