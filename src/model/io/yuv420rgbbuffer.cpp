#include "yuv420rgbbuffer.h"
#include <QFile>
#include <QDebug>

extern "C"
{
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/frame.h"
}

static enum AVPixelFormat bitDepth2PixelFormat( int iBitDepth )
{
    enum AVPixelFormat pixelFormat;
    if ( iBitDepth == 8 )
    {
        pixelFormat = AV_PIX_FMT_YUV420P;
    }
    else if ( iBitDepth == 10 )
    {
        pixelFormat = AV_PIX_FMT_YUV420P10LE;
    }
    else if ( iBitDepth == 16 )
    {
        pixelFormat = AV_PIX_FMT_YUV420P16LE;
    }
    return pixelFormat;
}


YUV420RGBBuffer::YUV420RGBBuffer()
{

    m_iBufferWidth = 0;
    m_iBufferHeight = 0;
    m_iFrameCount = -1;
    m_iBitDepth = -1;
    m_puhYUVBuffer = NULL;
    m_puhRGBBuffer = NULL;
    m_pFrameYUV = av_frame_alloc();
    m_pFrameRGB = av_frame_alloc();
    m_pConvertCxt = NULL;
}

YUV420RGBBuffer::~YUV420RGBBuffer()
{

    av_free(m_puhYUVBuffer);
    m_puhYUVBuffer = NULL;

    av_free(m_puhRGBBuffer);
    m_puhRGBBuffer = NULL;

    av_frame_free(&m_pFrameYUV);
    av_frame_free(&m_pFrameRGB);
    sws_freeContext(m_pConvertCxt);
}


bool YUV420RGBBuffer::openYUVFile( const QString& strYUVPath, int iWidth, int iHeight, int iBitDepth )
{

    /// if new size dosen't match current size, delete old one and create new one
    if( iWidth != m_iBufferWidth || iHeight != m_iBufferHeight || iBitDepth != m_iBitDepth )
    {
        enum AVPixelFormat pixelFormat = bitDepth2PixelFormat(iBitDepth);

        av_free(m_puhYUVBuffer);
        m_puhYUVBuffer = (uint8_t *)av_malloc(av_image_get_buffer_size(pixelFormat, iWidth, iHeight, 1) * sizeof(uint8_t));

        av_free(m_puhRGBBuffer);
        m_puhRGBBuffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, iWidth, iHeight, 1) * sizeof(uint8_t));

        av_image_fill_arrays(m_pFrameYUV->data, m_pFrameYUV->linesize, m_puhYUVBuffer, pixelFormat, iWidth, iHeight, 1);
        av_image_fill_arrays(m_pFrameRGB->data, m_pFrameRGB->linesize, m_puhRGBBuffer, AV_PIX_FMT_RGB32, iWidth, iHeight, 1);

        sws_freeContext(m_pConvertCxt);
        m_pConvertCxt = sws_getContext(iWidth, iHeight, pixelFormat, iWidth, iHeight, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    }


    m_iBufferWidth = iWidth;
    m_iBufferHeight = iHeight;
    m_iBitDepth = iBitDepth;

    /// set YUV file reader
    if( !m_cIOYUV.openYUVFilePath(strYUVPath) )
    {
        qCritical() << "YUV Buffer Initialization Fail";
        return false;
    }


    return true;

}

QPixmap* YUV420RGBBuffer::getFrame(int iFrameCount)
{
    QPixmap* pcFramePixmap = NULL;

    if( xReadFrame(iFrameCount) )
    {
        QImage cFrameImg(m_puhRGBBuffer, m_iBufferWidth, m_iBufferHeight, QImage::Format_RGB32 );
        m_cFramePixmap = QPixmap::fromImage(cFrameImg);
        pcFramePixmap = &m_cFramePixmap;
    }
    else
    {
        qCritical() << "Read YUV File Failure.";
    }

    return pcFramePixmap;
}

bool YUV420RGBBuffer::xReadFrame(int iFrameCount)
{
    if( iFrameCount < 0 )
        return false;
    m_iFrameCount = iFrameCount;
    enum AVPixelFormat pixelFormat = bitDepth2PixelFormat(m_iBitDepth);
    int iFrameSizeInByte = av_image_get_buffer_size(pixelFormat, m_iBufferWidth, m_iBufferHeight, 1);
    if( m_cIOYUV.seekTo(iFrameCount*iFrameSizeInByte) == false )
        return false;
    int iReadBytes = 0;   ///read

    iReadBytes = m_cIOYUV.readOneFrame(m_puhYUVBuffer, (uint)iFrameSizeInByte);

    if( iReadBytes != iFrameSizeInByte )
    {
        qCritical() << "Read YUV Frame Error";
        return false;
    }
    else
    {
        /// YUV to RGB conversion
        sws_scale(m_pConvertCxt, (const uint8_t *const *) m_pFrameYUV->data, m_pFrameYUV->linesize,
                  0, m_iBufferHeight, m_pFrameRGB->data, m_pFrameRGB->linesize);
        return true;
    }

}
