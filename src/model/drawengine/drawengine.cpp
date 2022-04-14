#include "drawengine.h"
#include <QPainter>
#include <iostream>
using namespace std;
DrawEngine::DrawEngine()
{
    m_dScale = 1.0;
    m_pcCurFrame = NULL;
}


QPixmap* DrawEngine::drawFrame( ComSequence* pcSequence, int iPoc, QPixmap *pcPixmap )
{
    ComFrame* pcFrame = pcSequence->getFramesInDisOrder().at(iPoc);
    m_pcCurFrame = pcFrame;
    int iLCUTotalNum = pcFrame->getLCUs().size();

    m_iMaxCUSize = pcSequence ->getMaxCUSize();

    /// draw original pic
    m_cDrawnPixmap = pcPixmap->scaled(pcPixmap->size()*m_dScale, Qt::KeepAspectRatio, Qt::FastTransformation);
    QPainter cPainter(&m_cDrawnPixmap);

    /***********************************************************************
     *               Followings are for drawing filters                    *
     ***********************************************************************/
    /// draw TU
    for( int iAddr = 0; iAddr < iLCUTotalNum; iAddr++ )
    {
        ComCU* pcLCU = pcFrame->getLCUs().at(iAddr);
        xDrawTU( &cPainter, pcLCU );
    }


    /// draw PU
    for( int iAddr = 0; iAddr < iLCUTotalNum; iAddr++ )
    {
        ComCU* pcLCU = pcFrame->getLCUs().at(iAddr);        
        xDrawPU( &cPainter, pcLCU );
    }

    /// draw CU
    QRect cScaledCUArea;
    for( int iAddr = 0; iAddr < iLCUTotalNum; iAddr++ )
    {
        ComCU* pcLCU = pcFrame->getLCUs().at(iAddr);
        xDrawCU( &cPainter, pcLCU );
    }

    /// draw CTU (i.e. LCU)
    for( int iAddr = 0; iAddr < iLCUTotalNum; iAddr++ )
    {
        ComCU* pcLCU = pcFrame->getLCUs().at(iAddr);
        int iPixelX = pcLCU->getX();
        int iPixelY = pcLCU->getY();
        cScaledCUArea.setCoords( iPixelX, iPixelY, (iPixelX+pcLCU->getWidth())-1, (iPixelY+pcLCU->getHeight())-1 );
        xScaleRect(&cScaledCUArea,&cScaledCUArea);
        m_cFilterLoader.drawCTU(&cPainter, pcLCU, m_dScale, &cScaledCUArea);
    }

    ///drawTile
    xDrawTile(&cPainter, pcFrame);

    ///draw Frame
    QRect cScaledFrameArea =  m_cDrawnPixmap.rect();
    m_cFilterLoader.drawFrame(&cPainter, pcFrame, m_dScale, &cScaledFrameArea);

    return &m_cDrawnPixmap;

}

bool DrawEngine::xDrawTile(QPainter *pcPainter, ComFrame *pcFrame)
{
    ComCU * iLCU = NULL;
    QRect cScaledTileArea;
    int iTileNum = pcFrame->getTiles().size();
    for(int i = 0; i < iTileNum; ++i)
    {
        ComTile* pcTile = pcFrame->getTiles().at(i);
        int iAddr = pcTile->getFirstCUAddr();
        iLCU = pcFrame ->getLCUs().at(iAddr);
        int iX = iLCU ->getX();
        int iY = iLCU ->getY();

        int iWidth  = pcTile->getWidth()  * m_iMaxCUSize;
        int iHeight = pcTile->getHeight() * m_iMaxCUSize;

        cScaledTileArea.setCoords(iX , iY , iX + iWidth ,iY + iHeight);
        xScaleRect(&cScaledTileArea, &cScaledTileArea);
        cScaledTileArea = cScaledTileArea.intersected(m_cDrawnPixmap.rect()).adjusted(0, 0, -1, -1);
        m_cFilterLoader.drawTile(pcPainter, pcTile, m_dScale, &cScaledTileArea);

    }
    return true;

}





void DrawEngine::mousePress(const QPointF *pcScaledPos, Qt::MouseButton eMouseBtn)
{
    QPainter cPainter(&m_cDrawnPixmap);
    QPointF  cUnscaledPos = *pcScaledPos/m_dScale;
    m_cFilterLoader.mousePress(&cPainter, m_pcCurFrame, &cUnscaledPos, pcScaledPos, m_dScale, eMouseBtn);
}

void DrawEngine::keyPress(int iKeyPressed)
{
    QPainter cPainter(&m_cDrawnPixmap);
    m_cFilterLoader.keyPress(&cPainter, m_pcCurFrame, iKeyPressed);
}

bool DrawEngine::xDrawPU( QPainter* pcPainter,  ComCU* pcCU )
{
    // when drawing hevc bitstream, non-empty PU array means leaf CU, then begin draw PU
    // when drawing hevc bitstream, PUs are all stored in LCU, begin draw directly
    if( !pcCU->getPUs().empty() )
    {
        /// traverse very PU in this Leaf-CU
        QRect cScaledPUArea;
        for( int iPUIdx = 0; iPUIdx < pcCU->getPUs().size(); iPUIdx++ )
        {
            ComPU* pcPU = pcCU->getPUs().at(iPUIdx);

            /// draw PU
            cScaledPUArea.setCoords( pcPU->getX(), pcPU->getY(), (pcPU->getX()+pcPU->getWidth())-1, (pcPU->getY()+pcPU->getHeight())-1 );
            xScaleRect(&cScaledPUArea, &cScaledPUArea);
            m_cFilterLoader.drawPU(pcPainter, pcPU, m_dScale, &cScaledPUArea);
        }

    }
    else
    {
        QVector<ComCU*>::iterator iter;
        for ( iter = pcCU->getSCUs().begin(); iter != pcCU->getSCUs().end(); iter++ )
        {
            xDrawPU ( pcPainter, *iter );
        }
    }
    return true;

}


bool DrawEngine::xDrawCU( QPainter* pcPainter,  ComCU* pcCU )
{

    if( pcCU->getSCUs().empty() )
    {

        /// draw CU
        QRect cScaledCUArea;
        cScaledCUArea.setCoords( pcCU->getX(), pcCU->getY(), (pcCU->getX()+pcCU->getWidth())-1, (pcCU->getY()+pcCU->getHeight())-1 );
        xScaleRect(&cScaledCUArea,&cScaledCUArea);
        m_cFilterLoader.drawCU(pcPainter, pcCU, m_dScale, &cScaledCUArea);

    }
    else
    {
        QVector<ComCU*>::iterator iter;
        for (iter = pcCU->getSCUs().begin(); iter != pcCU->getSCUs().end(); iter++ )
        {
            xDrawCU ( pcPainter, *iter );
        }
    }
    return true;

}


bool DrawEngine::xDrawTU(QPainter* pcPainter,  ComCU *pcCU )
{
    bool isValidTU = pcCU->getTURoot().isValid( pcCU->getX(),
                                                pcCU->getY(),
                                                pcCU->getX() + pcCU->getWidth() - 1,
                                                pcCU->getY() + pcCU->getHeight() - 1);

    // when drawing hevc bitstream:
    // empty CU array means leaf CU, and (isValidTU == true) only for leaf CU, then begin draw TU
    // when drawing hevc bitstream:
    // TURoot is stored in LCU, though SCU array is not empty but (isValidTU == true), then begin draw directly
    if( pcCU->getSCUs().empty() || isValidTU )
    {

        /// draw TU
        xDrawTUHelper(pcPainter, &pcCU->getTURoot());

    }
    else
    {
        QVector<ComCU*>::iterator iter;
        for (iter = pcCU->getSCUs().begin(); iter != pcCU->getSCUs().end(); iter++ )
        {
            xDrawCU ( pcPainter, *iter );
        }
    }
    return true;
}

bool DrawEngine::xDrawTUHelper( QPainter* pcPainter,  ComTU* pcTU )
{
    int iSubTUNum = pcTU->getTUs().size();
    if( iSubTUNum != 0 )
    {
        for(int i = 0; i < iSubTUNum; i++ )
        {
            xDrawTUHelper(pcPainter, pcTU->getTUs().at(i));
        }
    }
    else
    {
        QRect cScaledTUArea;
        cScaledTUArea.setCoords( pcTU->getX(), pcTU->getY(), (pcTU->getX()+pcTU->getWidth())-1, (pcTU->getY()+pcTU->getHeight())-1 );
        xScaleRect(&cScaledTUArea,&cScaledTUArea);
        m_cFilterLoader.drawTU(pcPainter, pcTU, m_dScale, &cScaledTUArea);

    }
    return true;
}


void DrawEngine::xScaleRect( QRect* rcUnscaled, QRect* rcScaled )
{
    rcScaled->setRect(rcUnscaled->x()*m_dScale, rcUnscaled->y()*m_dScale,
                      rcUnscaled->width()*m_dScale, rcUnscaled->height()*m_dScale);
}
