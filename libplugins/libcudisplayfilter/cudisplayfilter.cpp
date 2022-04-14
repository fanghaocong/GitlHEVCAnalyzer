#include "cudisplayfilter.h"
#include "model/modellocator.h"
#include <QDebug>
CUDisplayFilter::CUDisplayFilter(QObject *parent) :
    QObject(parent)
{
    setName("CU Structure");

    ///
    m_cConfigDialog.setWindowTitle("CU Structure Filter");
    m_cConfigDialog.addCheckbox("Display PU", "", &m_cConfig.getShowPU());
    m_cConfigDialog.addCheckbox("Only Disaly LCU", "", &m_cConfig.getShowLCUOnly());
    m_cConfigDialog.addColorPicker("PU Color", &m_cConfig.getPUColor());
    m_cConfigDialog.addColorPicker("Largest CU Color (LCU/CTU)", &m_cConfig.getLCUColor());
    m_cConfigDialog.addColorPicker("Leaf CU Color (Smallest CU)", &m_cConfig.getSCUColor());
    m_cConfigDialog.addSlider("Opaque", 0.1, 1.0, &m_cConfig.getOpaque() );

    /// init lcu pen
    m_cLCUPen.setStyle(Qt::SolidLine);
    m_cLCUPen.setWidth(3);
    m_cLCUPen.setBrush(QBrush(m_cConfig.getLCUColor()));

    /// init cu pen
    m_cCUPen.setStyle(Qt::SolidLine);
    m_cCUPen.setWidth(1);
    m_cCUPen.setBrush(QBrush(m_cConfig.getSCUColor()));

    /// init pu pen
    m_cPUPen.setStyle(Qt::DashLine);
    m_cPUPen.setWidth(1);
    m_cPUPen.setBrush(QBrush(m_cConfig.getPUColor()));

    /// selected CU
    m_pcSelectedCU = NULL;


}

bool CUDisplayFilter::config   (FilterContext* pcContext)
{
    m_cConfigDialog.exec();
    m_cConfig.applyOpaque();
    m_cLCUPen.setBrush(QBrush(m_cConfig.getLCUColor()));
    m_cCUPen.setBrush(QBrush(m_cConfig.getSCUColor()));
    m_cPUPen.setBrush(QBrush(m_cConfig.getPUColor()));
    return true;
}


bool CUDisplayFilter::drawCTU  (FilterContext *pcContext, QPainter *pcPainter,
                                ComCU *pcCTU, double dScale, QRect *pcScaledArea)
{
    pcPainter->setBrush(Qt::NoBrush);
    pcPainter->setPen(m_cLCUPen);
    pcPainter->drawRect(*pcScaledArea);
    return true;
}

bool CUDisplayFilter::drawCU   (FilterContext* pcContext, QPainter* pcPainter,
                                ComCU *pcCU, double dScale,  QRect* pcScaledArea)
{
    /// only show LCU
    if(m_cConfig.getShowLCUOnly())
        return true;

    /// Draw CU Rect


    if(pcCU == m_pcSelectedCU)      /// selected
    {
        QPen cSelectedPen = m_cCUPen;
        cSelectedPen.setWidth(m_cCUPen.width()+3);
        pcPainter->setBrush(QBrush(QColor(255,0,0,128)));
        pcPainter->setPen(cSelectedPen);
    }
    else
    {
        pcPainter->setBrush(Qt::NoBrush);
        pcPainter->setPen(m_cCUPen);
    }
    pcPainter->drawRect(*pcScaledArea);

    /// Draw PU
    if(m_cConfig.getShowPU())
    {
        pcPainter->setPen(m_cPUPen);

        QVector<ComPU*>::iterator iter;
        for ( iter = pcCU->getPUs().begin(); iter != pcCU->getPUs().end(); iter++ )
        {
            int x = (*iter)->getX() * dScale;
            int y = (*iter)->getY() * dScale;
            int width = (*iter)->getWidth() * dScale;
            int height = (*iter)->getHeight() * dScale;
            QRect *subScaledArea = new QRect(x, y, width, height);
            pcPainter->drawRect(*subScaledArea);
        }
    }


    return true;
}


bool CUDisplayFilter::mousePress(FilterContext *pcContext, QPainter *pcPainter, ComFrame *pcFrame, const QPointF *pcUnscaledPos, const QPointF *scaledPos, double dScale, Qt::MouseButton eMouseBtn)
{
    m_pcSelectedCU = pcContext->pcSelectionManager->getSCU(pcFrame, pcUnscaledPos);
    return true;
}

bool CUDisplayFilter::keyPress(FilterContext *pcContext, QPainter *pcPainter, ComFrame *pcFrame, int iKeyPressed)
{
    if(iKeyPressed == Qt::Key_Escape)
        m_pcSelectedCU = NULL;
    return true;
}
