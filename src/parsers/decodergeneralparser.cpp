#include "decodergeneralparser.h"
#include <QRegExp>
#include <QTextStream>
#include <QDebug>

/// rearrange frames into POC ascending order
static bool xFrameSortingOrder(ComFrame* pcFrameFirst, ComFrame* pcFrameSecond)
{
    return (pcFrameFirst->getPOC() < pcFrameSecond->getPOC());
}


DecoderGeneralParser::DecoderGeneralParser(QObject *parent) :
    QObject(parent)
{
}

static void readIntArray(QVector<int>* paiArr, QString* pstrArrStr)
{
    QString strTrimmed = pstrArrStr->trimmed();
    QTextStream strTextStream(&strTrimmed);
    int iPOC;
    while(!strTextStream.atEnd())
    {
        strTextStream >> iPOC;
        paiArr->push_back(iPOC);
    }
}

bool DecoderGeneralParser::parseFile(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    Q_ASSERT( pcSequence != NULL );

    if ( pcSequence->getEncoder() == "VTM" )
    {
        return parseFile_VVC(pcInputStream, pcSequence);
    }
    else if ( pcSequence->getEncoder() == "HM" )
    {
        return parseFile_HEVC(pcInputStream, pcSequence);
    }

    return false;
}

void DecoderGeneralParser::xSortByFrameCount( ComSequence* pcSequence )
{
    /// Get IDR groups
    QList<QVector<ComFrame*>::iterator> cIDRGroup;
    QVector<ComFrame*>::iterator cIter = pcSequence->getFramesInDisOrder().begin();
    while(cIter!=pcSequence->getFramesInDisOrder().end())
    {
        if ((*cIter)->getPOC() == 0)
        {
            cIDRGroup.push_back(cIter);
        }
        cIter++;
    }
    cIDRGroup.push_back(cIter);
    Q_ASSERT(cIDRGroup.size() >= 2);    /// at least one IDR group

    /// sort each IDRGroup
    for(int i = 0; i < cIDRGroup.size()-1; i++)
        qSort(cIDRGroup[i], cIDRGroup[i+1], xFrameSortingOrder);

    /// assign frame num
    for(int i = 0; i < pcSequence->getFramesInDisOrder().size(); i++)
    {
        pcSequence->getFramesInDisOrder().at(i)->setFrameCount(i);
    }
}

bool DecoderGeneralParser::parseFile_HEVC(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    QString strOneLine;
    QRegExp cMatchTarget;


    //HM software: Decoder Version [5.2][Windows][VS 1500][32 bit]
    cMatchTarget.setPattern("HM software: Decoder Version \\[([0-9.]+)\\].*");
    while( !pcInputStream->atEnd() ) {
        strOneLine = pcInputStream->readLine();
        if( cMatchTarget.indexIn(strOneLine) != -1 ) {
            QString strEncoderVersion = cMatchTarget.cap(1);
            pcSequence->setEncoderVersion(strEncoderVersion);
            break;
        }
    }


    // POC   18 TId: 0 ( B-SLICE, QP 33 ) [DT  0.005] [L0 17 16 15 14 ] [L1 17 16 15 14 ] [LC 17 16 15 14 ] [MD5:f2984e1c87633f1c2476eb2ddb844a53,(OK)]
    // read one frame

    ComFrame *pcFrame = NULL;
    cMatchTarget.setPattern("POC *(-?[0-9]+).*\\[DT *([0-9.]+) *\\] \\[L0(( -?[0-9]+){0,}) \\] \\[L1(( -?[0-9]+){0,}) \\] (\\[LC(( -?[0-9]+){0,}) \\])?");
    pcInputStream->readLine();///< Skip a empty line
    while( !pcInputStream->atEnd() )
    {

        while( !pcInputStream->atEnd() )
        {
            strOneLine = pcInputStream->readLine();
            if(cMatchTarget.indexIn(strOneLine) != -1)
                break;
        }
        if( cMatchTarget.indexIn(strOneLine) != -1 ) {
            pcFrame = new ComFrame(pcSequence);

            /// POC & Decoding time
            pcFrame->setPOC(cMatchTarget.cap(1).toInt());
            pcFrame->setTotalDecTime(cMatchTarget.cap(2).toDouble());

            /// L0 L1 LC
            QString strL0, strL1, strLC;
            strL0 = cMatchTarget.cap(3); strL1 = cMatchTarget.cap(5); strLC = cMatchTarget.cap(7);
            readIntArray(&pcFrame->getL0List(), &strL0);
            readIntArray(&pcFrame->getL1List(), &strL1);
            readIntArray(&pcFrame->getLCList(), &strLC);

            pcSequence->getFramesInDecOrder().push_back(pcFrame);
            pcSequence->getFramesInDisOrder().push_back(pcFrame);


        }
        else    // frames finish
        {

            pcSequence->setTotalFrames(pcSequence->getFramesInDisOrder().size());

            // Total Time:     2070.335 sec.

            cMatchTarget.setPattern(" Total Time: *([0-9.]+).*");

            {
                strOneLine = pcInputStream->readLine();
                if( cMatchTarget.indexIn(strOneLine) != -1 ) {
                    pcSequence->setTotalDecTime(cMatchTarget.cap(1).toDouble());
                    break;
                }
            }
        }

    }

    /// sort & calculate frame displaying order according to POC
    xSortByFrameCount(pcSequence);

    return true;
}

bool DecoderGeneralParser::parseFile_VVC(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    QString strOneLine;
    QRegExp cMatchTarget;


    // VVCSoftware: VTM Decoder Version 16.0 [Windows][GCC 8.1.0][64 bit] [SIMD=AVX2]
    cMatchTarget.setPattern("VVCSoftware: VTM Decoder Version ([0-9.]+).*");
    while( !pcInputStream->atEnd() ) {
        strOneLine = pcInputStream->readLine();
        if( cMatchTarget.indexIn(strOneLine) != -1 ) {
            QString strEncoderVersion = cMatchTarget.cap(1);
            pcSequence->setEncoderVersion(strEncoderVersion);
            break;
        }
    }

    // POC   12 LId:  0 TId: 0 ( TRAIL, P-SLICE, QP 44 ) [DT  0.014] [L0 11c 10 8 0] [L1] [:,(unk)]
    ComFrame *pcFrame = NULL;
    cMatchTarget.setPattern("POC *([0-9]+).*\\[DT *([0-9.]+)*\\] \\[L0(( [0-9]+c?){0,})\\] \\[L1(( [0-9]+c?){0,})\\]");
    pcInputStream->readLine(); ///< Skip a empty line
    while( !pcInputStream->atEnd() )
    {

        while( !pcInputStream->atEnd() )
        {
            strOneLine = pcInputStream->readLine();
            if(cMatchTarget.indexIn(strOneLine) != -1)
                break;
        }
        if( cMatchTarget.indexIn(strOneLine) != -1 ) {
            pcFrame = new ComFrame(pcSequence);

            /// POC & Decoding time
            pcFrame->setPOC(cMatchTarget.cap(1).toInt());
            pcFrame->setTotalDecTime(cMatchTarget.cap(2).toDouble());

            /// L0 L1
            QString strL0, strL1;
            strL0 = cMatchTarget.cap(3);
            strL1 = cMatchTarget.cap(5);
            if ( strL0.contains("c") )
            {
                strL0.replace("c", "");
            }
            if ( strL1.contains("c") )
            {
                strL1.replace("c", "");
            }
            readIntArray(&pcFrame->getL0List(), &strL0);
            readIntArray(&pcFrame->getL1List(), &strL1);

            pcSequence->getFramesInDecOrder().push_back(pcFrame);
            pcSequence->getFramesInDisOrder().push_back(pcFrame);


        }
        else    // frames finish
        {
            pcSequence->setTotalFrames(pcSequence->getFramesInDisOrder().size());

            // Total Time:     2070.335 sec.

            cMatchTarget.setPattern(" Total Time: *([0-9.]+).*");

            {
                if( cMatchTarget.indexIn(strOneLine) != -1 ) {
                    pcSequence->setTotalDecTime(cMatchTarget.cap(1).toDouble());
                    break;
                }
            }
        }

    }

    /// sort & calculate frame displaying order according to POC
    xSortByFrameCount(pcSequence);

    return true;
}
