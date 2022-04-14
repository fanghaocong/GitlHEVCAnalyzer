#include "predparser.h"
#include <QRegExp>
#include <QDebug>

PredParser::PredParser(QObject *parent) :
    QObject(parent)
{
}

bool PredParser::parseFile(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    Q_ASSERT( pcSequence != NULL );

    QString strOneLine;
    QRegExp cMatchTarget;


    /// <1,1> 0 0 1 1 0
    /// read one LCU
    ComFrame* pcFrame = NULL;
    ComCU* pcLCU = NULL;
    cMatchTarget.setPattern("^<(-?[0-9]+),([0-9]+)> (.*)");
    QTextStream cPredInfoStream;
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
            pcLCU = pcFrame->getLCUs().at(iAddr);


            ///
            QString strCUInfo = cMatchTarget.cap(3);

            cPredInfoStream.setString(&strCUInfo, QIODevice::ReadOnly );

            xReadPredMode(&cPredInfoStream, pcLCU, pcSequence);

        }

    }
    return true;
}


bool PredParser::xReadPredMode(QTextStream* pcPredInfoStream, ComCU* pcCU, ComSequence* pcSequence)
{
    if ( pcSequence->getEncoder() == "HM" )
    {
        return xReadPredMode_HEVC(pcPredInfoStream, pcCU);
    }
    else if ( pcSequence->getEncoder() == "VTM" )
    {
        return xReadPredMode_VVC(pcPredInfoStream, pcCU);
    }

    return false;
}

bool PredParser::xReadPredMode_HEVC(QTextStream* pcPredInfoStream, ComCU* pcCU)
{
    if( !pcCU->getSCUs().empty() )
    {
        /// non-leaf node : recursive reading for children
        xReadPredMode_HEVC(pcPredInfoStream, pcCU->getSCUs().at(0));
        xReadPredMode_HEVC(pcPredInfoStream, pcCU->getSCUs().at(1));
        xReadPredMode_HEVC(pcPredInfoStream, pcCU->getSCUs().at(2));
        xReadPredMode_HEVC(pcPredInfoStream, pcCU->getSCUs().at(3));
    }
    else
    {
        /// leaf node : read data
        int iPredMode;
        for(int i = 0; i < pcCU->getPUs().size(); i++)
        {


            Q_ASSERT(pcPredInfoStream->atEnd() == false);

            *pcPredInfoStream >> iPredMode;
            ComPU* pcPU = pcCU->getPUs().at(i);
            pcPU->setPredMode((PredMode)iPredMode);
        }
    }
    return true;
}


bool PredParser::xReadPredMode_VVC(QTextStream* pcPredInfoStream, ComCU* pcCU)
{
    int iPredMode;
    for(int i = 0; i < pcCU->getPUs().size(); i++)
    {
        Q_ASSERT(pcPredInfoStream->atEnd() == false);

        *pcPredInfoStream >> iPredMode;
        ComPU* pcPU = pcCU->getPUs().at(i);
        pcPU->setPredMode((PredMode)iPredMode);
    }
}
