#include "cupuparser.h"
#include <QTextStream>
#include <QRegExp>
#include <QtAlgorithms>
#include <QDebug>

#include "parserutilities.h"

#define CU_SLIPT_FLAG 99      ///< CU splitting flag in file

/// for CU sorting in Addr ascending order
static bool xCUSortingOrder(const ComCU* pcCUFirst, const ComCU* pcCUSecond)
{
    return (*pcCUFirst < *pcCUSecond);
}

CUPUParser::CUPUParser(QObject *parent) :
    QObject(parent)
{
}


bool CUPUParser::parseFile(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    Q_ASSERT( pcSequence != NULL );

    int iSeqWidth = pcSequence->getWidth();
    int iMaxCUSize = pcSequence->getMaxCUSize();
    int iCUOneRow = (iSeqWidth+iMaxCUSize-1)/iMaxCUSize;

    ////
    QString strOneLine;
    QRegExp cMatchTarget;


    /// <1,1> 99 0 0 5 0
    /// <1,1> x1 y1 width1 height1 x2 y2 width2 height2 ...
    /// read one LCU
    ComFrame* pcFrame = NULL;
    ComCU* pcLCU = NULL;
    int iLCUSize = pcSequence->getMaxCUSize();
    cMatchTarget.setPattern("^<(-?[0-9]+),([0-9]+)> (.*) ");
    QTextStream cCUInfoStream;
    int iDecOrder = -1;
    int iLastPOC  = -1;
    while( !pcInputStream->atEnd() )
    {

        strOneLine = pcInputStream->readLine();
        if( cMatchTarget.indexIn(strOneLine) != -1 )
        {

            /// poc and lcu addr
            int iPoc = cMatchTarget.cap(1).toInt();
            iDecOrder += (iLastPOC != iPoc);
            iLastPOC = iPoc;
            pcFrame = pcSequence->getFramesInDecOrder().at(iDecOrder);
            int iAddr = cMatchTarget.cap(2).toInt();
            pcLCU = new ComCU(pcFrame);
            pcLCU->setAddr(iAddr);
            pcLCU->setFrame(pcFrame);
            pcLCU->setDepth(0);
            pcLCU->setZorder(0);
            pcLCU->setWidth(iLCUSize);
            pcLCU->setHeight(iLCUSize);
            int iPixelX = (pcLCU->getAddr()%iCUOneRow)*iMaxCUSize;
            int iPixelY = (pcLCU->getAddr()/iCUOneRow)*iMaxCUSize;
            pcLCU->setX(iPixelX);
            pcLCU->setY(iPixelY);

            /// recursively parse the CU&PU quard-tree structure
            QString strCUInfo = cMatchTarget.cap(3);
            cCUInfoStream.setString( &strCUInfo, QIODevice::ReadOnly );
            if( xReadInCUMode( &cCUInfoStream, pcLCU, pcSequence ) == false )
                return false;
            pcFrame->getLCUs().push_back(pcLCU);
        }

        /// sort LCU in ascendning order
        qSort(pcFrame->getLCUs().begin(), pcFrame->getLCUs().end(), xCUSortingOrder);
    }

    return true;
}


bool CUPUParser::xReadInCUMode(QTextStream* pcCUInfoStream, ComCU* pcCU, ComSequence* pcSequence)
{
    if ( pcSequence->getEncoder() == "HM" )
    {
        return xReadInCUMode_HEVC(pcCUInfoStream, pcCU);
    }
    else if ( pcSequence->getEncoder() == "VTM" )
    {
        return xReadInCUMode_VVC(pcCUInfoStream, pcCU);
    }

    return false;
}


bool CUPUParser::xReadInCUMode_HEVC(QTextStream* pcCUInfoStream, ComCU* pcCU)
{
    int iCUMode;
    if( pcCUInfoStream->atEnd() )
    {
        qCritical() << "CUPUParser Error! Illegal CU/PU Mode!";
        return false;
    }
    *pcCUInfoStream >> iCUMode;

    if( iCUMode == CU_SLIPT_FLAG )
    {
        int iMaxDepth = pcCU->getFrame()->getSequence()->getMaxCUDepth();
        int iTotalNumPart = 1 << ( (iMaxDepth-pcCU->getDepth()) << 1 );
        /// non-leaf node : add 4 children CUs
        for(int i = 0; i < 4; i++)
        {
            ComCU* pcChildNode = new ComCU(pcCU->getFrame());
            pcChildNode->setAddr(pcCU->getAddr());
            pcChildNode->setDepth(pcCU->getDepth()+1);
            pcChildNode->setZorder( pcCU->getZorder() + (iTotalNumPart/4)*i );
            pcChildNode->setWidth(pcCU->getWidth()/2);
            pcChildNode->setHeight(pcCU->getHeight()/2);
            int iSubCUX = pcCU->getX() + i%2 * (pcCU->getWidth()/2);
            int iSubCUY = pcCU->getY() + i/2 * (pcCU->getHeight()/2);
            pcChildNode->setX(iSubCUX);
            pcChildNode->setY(iSubCUY);
            pcCU->getSCUs().push_back(pcChildNode);
            xReadInCUMode_HEVC(pcCUInfoStream, pcChildNode);
        }
    }
    else
    {
        /// leaf node : create PUs and write the PU Mode for it
        pcCU->setPartSize((PartSize)iCUMode);

        int iPUCount = ComCU::getPUNum((PartSize)iCUMode);
        for(int i = 0; i < iPUCount; i++)
        {
            ComPU* pcPU = new ComPU(pcCU);
            int iPUOffsetX, iPUOffsetY, iPUWidth, iPUHeight;
            ComCU::getPUOffsetAndSize(pcCU->getWidth(), (PartSize)iCUMode, i, iPUOffsetX, iPUOffsetY, iPUWidth, iPUHeight);
            int iPUX = pcCU->getX() + iPUOffsetX;
            int iPUY = pcCU->getY() + iPUOffsetY;
            pcPU->setX(iPUX);
            pcPU->setY(iPUY);
            pcPU->setWidth(iPUWidth);
            pcPU->setHeight(iPUHeight);
            pcCU->getPUs().push_back(pcPU);
        }

    }
    return true;
}

bool CUPUParser::xReadInCUMode_VVC(QTextStream* pcCUInfoStream, ComCU* pcCU)
{
    QVector<int>* arr = new QVector<int>;
    ParserUtilities::getInstance()->stream2IntAray(pcCUInfoStream, arr);
    if ( arr->size() % 4 != 0)
    {
        qCritical() << "CUPUParser Error! Illegal CU/PU Data!";
        return false;
    }
    int iX, iY, iWidth, iHeight;

    // add all leaf CUs/PUs in LCU->getSCUs()/getPUs()
    for ( int i=0; i < arr->size(); i+=4 )
    {
        iX = arr->at(i);
        iY = arr->at(i+1);
        iWidth = arr->at(i+2);
        iHeight = arr->at(i+3);

        ComCU* pcSCU = new ComCU(pcCU->getFrame());
        pcSCU->setAddr(pcCU->getAddr());
        pcSCU->setX(iX);
        pcSCU->setY(iY);
        pcSCU->setWidth(iWidth);
        pcSCU->setHeight(iHeight);
        pcCU->getSCUs().push_back(pcSCU);

        ComPU* pcPU = new ComPU(pcCU);
        pcPU->setX(iX);
        pcPU->setY(iY);
        pcPU->setWidth(iWidth);
        pcPU->setHeight(iHeight);
        pcCU->getPUs().push_back(pcPU);
    }

    return true;
}
