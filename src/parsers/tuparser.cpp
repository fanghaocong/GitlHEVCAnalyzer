#include "tuparser.h"
#include <QRegExp>
#include <QDebug>

#include "parserutilities.h"

#define TU_SLIPT_FLAG 99

TUParser::TUParser(QObject *parent):
    QObject(parent)
{
}


bool TUParser::parseFile(QTextStream* pcInputStream, ComSequence* pcSequence)
{
    Q_ASSERT( pcSequence != NULL );

    QString strOneLine;
    QRegExp cMatchTarget;


    /// <1,1> 1 -3 0 1 -3 0 1 -3 0 1 1 0 1 -3 0
    /// <1,3> 400 32 16 32 384 64 16 32 400 64 16 32
    /// read one LCU
    ComFrame* pcFrame = NULL;
    ComCU* pcLCU = NULL;
    cMatchTarget.setPattern("^<(-?[0-9]+),([0-9]+)> (.*) ");
    QTextStream cTUInfoStream;
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

            QString strTUInfo = cMatchTarget.cap(3);
            cTUInfoStream.setString( &strTUInfo, QIODevice::ReadOnly );

            xReadTU(&cTUInfoStream, pcLCU, pcSequence);
        }
    }
    return true;
}


bool TUParser::xReadTU(QTextStream* pcCUInfoStream, ComCU* pcTU, ComSequence* pcSequence)
{
    if ( pcSequence->getEncoder() == "HM" )
    {
        return xReadTU_HEVC(pcCUInfoStream, pcTU);
    }
    else if ( pcSequence->getEncoder() == "VTM" )
    {
        return xReadTU_VVC(pcCUInfoStream, pcTU);
    }

    return false;
}


bool TUParser::xReadTU_HEVC(QTextStream* pcTUInfoStream, ComCU* pcCU)
{
    if( !pcCU->getSCUs().empty() )
    {
        /// non-leaf CU node : continue to leaf CU
        xReadTU_HEVC(pcTUInfoStream, pcCU->getSCUs().at(0));
        xReadTU_HEVC(pcTUInfoStream, pcCU->getSCUs().at(1));
        xReadTU_HEVC(pcTUInfoStream, pcCU->getSCUs().at(2));
        xReadTU_HEVC(pcTUInfoStream, pcCU->getSCUs().at(3));
    }
    else
    {
        /// leaf CU node : read TU
        ComTU* pcTURoot = &pcCU->getTURoot();
        pcTURoot->setX(pcCU->getX());
        pcTURoot->setY(pcCU->getY());
        pcTURoot->setWidth(pcCU->getWidth());
        pcTURoot->setHeight(pcCU->getHeight());
        xReadTUHelper(pcTUInfoStream, &(pcCU->getTURoot()));
    }
    return true;
}


bool TUParser::xReadTU_VVC(QTextStream* pcTUInfoStream, ComCU* pcCU)
{
    QVector<int>* arr = new QVector<int>;
    ParserUtilities::getInstance()->stream2IntAray(pcTUInfoStream, arr);

    if ( arr->size() % 4 != 0)
    {
        qCritical() << "TUParser Error! Illegal TU Data!";
        return false;
    }

    ComTU* pcTURoot = &pcCU->getTURoot();
    pcTURoot->setX(pcCU->getX());
    pcTURoot->setY(pcCU->getY());
    pcTURoot->setWidth(pcCU->getWidth());
    pcTURoot->setHeight(pcCU->getHeight());

    int iTUX, iTUY, iTUWidth, iTUHeight;
    for ( int i = 0; i < arr->size(); i+=4 )
    {
        iTUX = arr->at(i);
        iTUY = arr->at(i+1);
        iTUWidth = arr->at(i+2);
        iTUHeight = arr->at(i+3);

        ComTU* pcChildNode = new ComTU();
        pcChildNode->setX(iTUX);
        pcChildNode->setY(iTUY);
        pcChildNode->setWidth(iTUWidth);
        pcChildNode->setHeight(iTUHeight);
        pcTURoot->getTUs().push_back(pcChildNode);
    }

    return true;
}


bool TUParser::xReadTUHelper(QTextStream* pcCUInfoStream, ComTU* pcTU)
{
    int iTUMode;
    if( pcCUInfoStream->atEnd() )
    {
        qCritical() << "TUParser Error! Illegal TU Mode!";
        return false;
    }
    *pcCUInfoStream >> iTUMode;

    if( iTUMode == TU_SLIPT_FLAG )
    {
        /// non-leaf node : add 4 children CUs
        for(int i = 0; i < 4; i++)
        {
            ComTU* pcChildNode = new ComTU();
            pcChildNode->setWidth(pcTU->getWidth()/2);
            pcChildNode->setHeight(pcTU->getHeight()/2);
            int iSubCUX = pcTU->getX() + i%2 * (pcTU->getWidth()/2);
            int iSubCUY = pcTU->getY() + i/2 * (pcTU->getHeight()/2);
            pcChildNode->setX(iSubCUX);
            pcChildNode->setY(iSubCUY);
            pcTU->getTUs().push_back(pcChildNode);
            xReadTUHelper(pcCUInfoStream, pcChildNode);
        }
    }
    else
    {
        /// leaf TU node : DO NOTHING
    }
    return true;
}
