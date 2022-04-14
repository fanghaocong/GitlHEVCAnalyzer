#ifndef YUV420RGBBUFFER_H
#define YUV420RGBBUFFER_H

#include <QObject>
#include <QPixmap>
#include <QFile>
#include <QMap>
#include "ioyuv.h"
#include "gitldef.h"

// declared in ffmpeg lib
class AVFrame;
class SwsContext;
//enum AVPixelFormat:int;


class YUV420RGBBuffer : public QObject
{
    Q_OBJECT
public:
    explicit YUV420RGBBuffer();
    ~YUV420RGBBuffer();

    bool openYUVFile( const QString& strYUVPath, int iWidth, int iHeight, int iBitDepth );
    QPixmap* getFrame(int iFrameCount);


    ADD_CLASS_FIELD(int, iBufferWidth, getBufferWidth, setBufferWidth)
    ADD_CLASS_FIELD(int, iBufferHeight, getBufferHeight, setBufferHeight)
    ADD_CLASS_FIELD(int, iBitDepth, getBitDepth, setBitDepth)
    ADD_CLASS_FIELD(int, iFrameCount, getFrameCount, setFrameCount)

    ADD_CLASS_FIELD_PRIVATE(QPixmap, cFramePixmap)
    ADD_CLASS_FIELD_PRIVATE(AVFrame*, pFrameYUV)
    ADD_CLASS_FIELD_PRIVATE(AVFrame*, pFrameRGB)
    ADD_CLASS_FIELD_PRIVATE(SwsContext*, pConvertCxt)
    ADD_CLASS_FIELD_PRIVATE(uint8_t*,  puhYUVBuffer)
    ADD_CLASS_FIELD_PRIVATE(uint8_t*,  puhRGBBuffer)
    ADD_CLASS_FIELD_PRIVATE(IOYUV,   cIOYUV)



protected:
    bool xReadFrame(int iFrameCount);
//    void xYuv2rgb(uchar* puhYUV, uchar* puhRGB, int iWidth, int iHeight);
//    void x16to8BitClip(uchar* puh8BitYUV, const uchar* puh16BitYUV, const long lSizeInUnitCount);

signals:

public slots:

};

#endif // YUVBUFFER_H
