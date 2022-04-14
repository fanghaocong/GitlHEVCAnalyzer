#include "intraparser.h"
#include <QRegExp>
#include <QDebug>
IntraParser::IntraParser(QObject *parent) :
    QObject(parent)
{
}

bool IntraParser::parseFile(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    Q_ASSERT( pcSequence != NULL );

    QString strOneLine;
    QRegExp cMatchTarget;


    /// <0,8> 8 36 31 36 30 36 31 36 0 36 2 36 1 36 0 36 1 36
    /// read one LCU
    ComFrame* pcFrame = NULL;
    ComCU* pcLCU = NULL;
    cMatchTarget.setPattern("^<(-?[0-9]+),([0-9]+)> (.*)");
    QTextStream cIntraDirInfoStream;
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
            QString strIntraDir = cMatchTarget.cap(3);
            cIntraDirInfoStream.setString( &strIntraDir, QIODevice::ReadOnly );
            xReadIntraMode(&cIntraDirInfoStream, pcLCU, pcSequence);

        }

    }
    return true;
}


bool IntraParser::xReadIntraMode(QTextStream* pcCUInfoStream, ComCU* pcCU, ComSequence* pcSequence)
{
    if ( pcSequence->getEncoder() == "HM" )
    {
        return xReadIntraMode_HEVC(pcCUInfoStream, pcCU);
    }
    else if ( pcSequence->getEncoder() == "VTM" )
    {
        return xReadIntraMode_VVC(pcCUInfoStream, pcCU);
    }

    return false;
}

bool IntraParser::xReadIntraMode_HEVC(QTextStream* pcCUInfoStream, ComCU* pcCU)
{
    if( !pcCU->getSCUs().empty() )
    {
        /// non-leaf node : recursive reading for children
        xReadIntraMode_HEVC(pcCUInfoStream, pcCU->getSCUs().at(0));
        xReadIntraMode_HEVC(pcCUInfoStream, pcCU->getSCUs().at(1));
        xReadIntraMode_HEVC(pcCUInfoStream, pcCU->getSCUs().at(2));
        xReadIntraMode_HEVC(pcCUInfoStream, pcCU->getSCUs().at(3));
    }
    else
    {
        /// leaf node : read data
        int iIntraDirLuma;
        int iIntraDirChroma;
        for(int i = 0; i < pcCU->getPUs().size(); i++)
        {
            Q_ASSERT(pcCUInfoStream->atEnd()==false);
            *pcCUInfoStream >> iIntraDirLuma >> iIntraDirChroma;
            pcCU->getPUs().at(i)->setIntraDirLuma(iIntraDirLuma);
            pcCU->getPUs().at(i)->setIntraDirChroma(iIntraDirChroma);
        }
    }
    return true;
}


bool IntraParser::xReadIntraMode_VVC(QTextStream* pcCUInfoStream, ComCU* pcCU)
{
    for(int i = 0; i < pcCU->getPUs().size(); i++)
    {
        int iIntraDirLuma;
        int iIntraDirChroma;

        Q_ASSERT(pcCUInfoStream->atEnd()==false);
        *pcCUInfoStream >> iIntraDirLuma >> iIntraDirChroma;
        pcCU->getPUs().at(i)->setIntraDirLuma(iIntraDirLuma);
        pcCU->getPUs().at(i)->setIntraDirChroma(iIntraDirChroma);
    }

    return true;
}
