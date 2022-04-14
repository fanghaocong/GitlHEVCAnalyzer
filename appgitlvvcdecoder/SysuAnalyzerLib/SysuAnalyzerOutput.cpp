#include "SysuAnalyzerOutput.h"
#include "CommonLib/UnitTools.h"

using namespace std;

SysuAnalyzerOutput* SysuAnalyzerOutput::m_instance = NULL;

SysuAnalyzerOutput::SysuAnalyzerOutput()
{
    m_cSpsOut.open      ("decoder_sps.txt",  ios::out);
    m_cCUPUOutput.open  ("decoder_cupu.txt", ios::out);
    m_cTUOutput.open    ("decoder_tu.txt",   ios::out);
    m_cPredOutput.open  ("decoder_pred.txt", ios::out);
    m_cMVOutput.open    ("decoder_mv.txt",   ios::out);
    m_cMergeOutput.open ("decoder_merge.txt",ios::out);
    m_cIntraOutput.open ("decoder_intra.txt",ios::out);
//    m_cLCUBitOutput.open("decoder_bit_lcu.txt",  ios::out);
//    m_cSCUBitOutput.open("decoder_bit_scu.txt",  ios::out);
}

SysuAnalyzerOutput::~SysuAnalyzerOutput()
{
    m_cSpsOut.close();
    m_cCUPUOutput.close();
    m_cTUOutput.close();
    m_cPredOutput.close();
    m_cMVOutput.close();
    m_cMergeOutput.close();
    m_cIntraOutput.close();
//    m_cLCUBitOutput.close();
//    m_cSCUBitOutput.close();
}

SysuAnalyzerOutput* SysuAnalyzerOutput::getInstance()
{
    if ( m_instance == NULL )
    {
        m_instance = new SysuAnalyzerOutput();
    }
    return m_instance;
}

void SysuAnalyzerOutput::writeOutSps( SPS* sps )
{
    m_cSpsOut << "Resolution:"   << sps->getMaxPicWidthInLumaSamples() << "x" << sps->getMaxPicHeightInLumaSamples() << endl;
    m_cSpsOut << "CTU Size:"  << sps->getCTUSize() << endl;
    m_cSpsOut << "Input Bit Depth:"  << sps->getBitDepth(CHANNEL_TYPE_LUMA) << endl;
}

void SysuAnalyzerOutput::writeOutCTUInfo( CodingStructure& cs, const UnitArea& ctuArea )
{
    int iPoc = cs.slice->getPOC();
    int iAddr = getCtuAddr( ctuArea.lumaPos(), *( cs.pcv ) );

    m_cCUPUOutput << "<" << iPoc << "," << iAddr << ">" << " ";
    m_cTUOutput << "<" << iPoc << "," << iAddr << ">" << " ";
    m_cPredOutput << "<" << iPoc << "," << iAddr << ">" << " ";
    m_cMVOutput << "<" << iPoc << "," << iAddr << ">" << " ";
    m_cMergeOutput << "<" << iPoc << "," << iAddr << ">" << " ";
    m_cIntraOutput << "<" << iPoc << "," << iAddr << ">" << " ";
//    m_cLCUBitOutput << "<" << iPoc << "," << iAddr << ">" << " ";
//    m_cLCUBitOutput << "<" << iPoc << "," << iAddr << ">" << " ";

    xWriteOutCUInfo( cs, ctuArea );
    xWriteOutTUInfo( cs, ctuArea );
    xWriteOutPredInfo( cs, ctuArea );
    xWriteOutMVInfo( cs, ctuArea );
    xWriteOutIntraInfo( cs, ctuArea );
    xWriteOutMergeInfo( cs, ctuArea );

    m_cCUPUOutput << endl;
    m_cTUOutput << endl;
    m_cPredOutput << endl;
    m_cMVOutput << endl;
    m_cMergeOutput << endl;
    m_cIntraOutput << endl;
//    m_cLCUBitOutput << endl;
//    m_cSCUBitOutput << endl;
}

void SysuAnalyzerOutput::xWriteOutCUInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
{
    for( auto &currCU : cs.traverseCUs( CS::getArea( cs, ctuArea, channelTyle ), channelTyle ) )
    {
        int x = currCU.blocks[CHANNEL_TYPE_LUMA].x;
        int y = currCU.blocks[CHANNEL_TYPE_LUMA].y;
        int width = currCU.blocks[CHANNEL_TYPE_LUMA].width;
        int height = currCU.blocks[CHANNEL_TYPE_LUMA].height;
        m_cCUPUOutput << x << " " << y << " " << width << " " << height << " ";
    }

}

void SysuAnalyzerOutput::xWriteOutTUInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
{
    for( auto &currCU : cs.traverseCUs( CS::getArea( cs, ctuArea, channelTyle ), channelTyle ) )
    {
        for( auto &currTU : CU::traverseTUs( currCU ) )
        {
            int x = currTU.blocks[CHANNEL_TYPE_LUMA].x;
            int y = currTU.blocks[CHANNEL_TYPE_LUMA].y;
            int width = currTU.blocks[CHANNEL_TYPE_LUMA].width;
            int height = currTU.blocks[CHANNEL_TYPE_LUMA].height;
            m_cTUOutput << x << " " << y << " " << width << " " << height << " ";
        }
    }
}

void SysuAnalyzerOutput::xWriteOutPredInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
{
    for( auto &currCU : cs.traverseCUs( CS::getArea( cs, ctuArea, channelTyle ), channelTyle ) )
    {
        PredMode ePred = currCU.predMode;
        int iPred;

        switch(ePred) {
        case MODE_INTER:
            iPred = 1;
            break;
        case MODE_INTRA:
            iPred = 2;
            break;
        case MODE_IBC:  // not supported
        case MODE_PLT:  // not supported
        default:
            iPred = 15;
        }

        m_cPredOutput << iPred << " ";
    }
}

void SysuAnalyzerOutput::xWriteOutMVInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
{
    Mv rcMv;
    for( auto &currCU : cs.traverseCUs( CS::getArea( cs, ctuArea, channelTyle ), channelTyle ) )
    {
        for( auto &currPU : CU::traversePUs( currCU ) )
        {
            int iInterDir = currPU.interDir; ///< Inter direction: 1--List 0 only, 2--List 1 only, 3--List 0&1(bi-direction), other--Invalid
            if (iInterDir < 1 || iInterDir > 3)
            {
                iInterDir = 0;
            }

            int iRefIdx = -1;
            m_cMVOutput << iInterDir << " ";

            if (iInterDir == 1)
            {
                rcMv = currPU.mv[REF_PIC_LIST_0];
                iRefIdx = currPU.refIdx[REF_PIC_LIST_0];
                m_cMVOutput << currPU.cs->slice->getRefPOC(REF_PIC_LIST_0, iRefIdx) << " " << rcMv.getHor() << " " << rcMv.getVer() << " ";
            }
            else if ( iInterDir == 2)
            {
                rcMv = currPU.mv[REF_PIC_LIST_1];
                iRefIdx = currPU.refIdx[REF_PIC_LIST_1];
                m_cMVOutput << currPU.cs->slice->getRefPOC(REF_PIC_LIST_1, iRefIdx) << " " << rcMv.getHor() << " " << rcMv.getVer() << " ";
            }
            else if ( iInterDir == 3)
            {
                rcMv = currPU.mv[REF_PIC_LIST_0];
                iRefIdx = currPU.refIdx[REF_PIC_LIST_0];
                m_cMVOutput << currPU.cs->slice->getRefPOC(REF_PIC_LIST_0, iRefIdx) << " " << rcMv.getHor() << " " << rcMv.getVer() << " ";

                rcMv = currPU.mv[REF_PIC_LIST_1];
                iRefIdx = currPU.refIdx[REF_PIC_LIST_1];
                m_cMVOutput << currPU.cs->slice->getRefPOC(REF_PIC_LIST_1, iRefIdx) << " " << rcMv.getHor() << " " << rcMv.getVer() << " ";
            }
            else
            {
                // do nothing
            }
        }
    }
}

void SysuAnalyzerOutput::xWriteOutMergeInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
{
    for( auto &currCU : cs.traverseCUs( CS::getArea( cs, ctuArea, channelTyle ), channelTyle ) )
    {
        for( auto &currPU : CU::traversePUs( currCU ) )
        {
            bool bMergeFlag = currPU.mergeFlag;
            int iMergeIndex = currPU.mergeIdx;
            if (bMergeFlag)
            {
                m_cMergeOutput << iMergeIndex << " ";
            }
            else
            {
                m_cMergeOutput << -1 << " ";
            }
        }
    }
}

void SysuAnalyzerOutput::xWriteOutIntraInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
{
    for( auto &currCU : cs.traverseCUs( CS::getArea( cs, ctuArea, channelTyle ), channelTyle ) )
    {
        for( auto &currPU : CU::traversePUs( currCU ) )
        {
            int iLumaIntraDir = currPU.intraDir[CHANNEL_TYPE_LUMA];
            int iChromaIntraDic = currPU.intraDir[CHANNEL_TYPE_CHROMA];
            m_cIntraOutput << iLumaIntraDir << " " << iChromaIntraDic << " ";
        }
    }

}

//void SysuAnalyzerOutput::xWriteOutLCUBitInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
//{

//}

//void SysuAnalyzerOutput::xWriteOutSCUBitInfo( CodingStructure& cs, const UnitArea& ctuArea, ChannelType channelTyle )
//{

//}
