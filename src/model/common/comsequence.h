#ifndef COMSEQUENCE_H
#define COMSEQUENCE_H
#include <QString>
#include <QMetaType>
#include "comframe.h"

enum YUVRole
{
    YUV_NONE,
    YUV_PREDICTED,
    YUV_RESIDUAL,
    YUV_RECONSTRUCTED
};

Q_DECLARE_METATYPE(YUVRole)

/*!
 * \brief This class represents a video sequence
 */

class ComSequence
{
public:
    explicit ComSequence();
    ~ComSequence();

    /*! Initialization
      */
    void init();

    /*! Frames in this sequence */
    ADD_CLASS_FIELD(QVector<ComFrame*>, cFramesInDisOrder, getFramesInDisOrder, setFramesInDisOrder)    /// frames in displaying order    0 1 2 3 4 ...
    ADD_CLASS_FIELD(QVector<ComFrame*>, cFramesInDecOrder, getFramesInDecOrder, setFramesInDecOrder)    /// frames in decoding order      0 4 2 1 3 ...


    /*! Sequence Parameter Set */
    ADD_CLASS_FIELD( QString, strFileName, getFileName, setFileName )     /// filename
    ADD_CLASS_FIELD( int, iWidth, getWidth, setWidth )                    /// width
    ADD_CLASS_FIELD( int, iHeight, getHeight, setHeight )                 /// height
    ADD_CLASS_FIELD( int, iTotalFrames, getTotalFrames, setTotalFrames )  /// total frame count

    ADD_CLASS_FIELD( int, iMaxCUSize, getMaxCUSize, setMaxCUSize )      /// max CU size (64 as general)
    ADD_CLASS_FIELD( int, iMaxCUDepth, getMaxCUDepth, setMaxCUDepth )   /// max CU Tree depth (4 as general)
    ADD_CLASS_FIELD( int, iMinTUDepth, getMaxIntraTUDepth, setMaxIntraTUDepth )   /// max Intra TU depth
    ADD_CLASS_FIELD( int, iMaxTUDepth, getMaxInterTUDepth, setMaxInterTUDepth )   /// max Inter TU depth
    ADD_CLASS_FIELD( int, iInputBitDepth, getInputBitDepth, setInputBitDepth)     /// YUV bit depth

    /*! Decoded File Location */
    ADD_CLASS_FIELD( QString, strDecodingFolder, getDecodingFolder, setDecodingFolder)

    /*! Currently Displaying YUV (Predicted, Residual or Reconstructed)*/
    ADD_CLASS_FIELD( YUVRole, eYUVRole, getYUVRole, setYUVRole)


    /*!
     * Optional info
     * Obsolescent
     */
    ADD_CLASS_FIELD(QString, strEncoder, getEncoder, setEncoder)
    ADD_CLASS_FIELD(QString, strEncoderVersion, getEncoderVersion, setEncoderVersion) ///
    ADD_CLASS_FIELD(double, dTotalDecTime, getTotalDecTime, setTotalDecTime)
    ADD_CLASS_FIELD(double, dPSNR, getPSNR, setPSNR)
    ADD_CLASS_FIELD(double, dBitrate, getBitrate, setBitrate)
    ADD_CLASS_FIELD(double, dTotalEncTime, getTotalEncTime, setTotalEncTime)
    ADD_CLASS_FIELD(double, dSameCUModePercent, getSameCUModePercent, setSameCUModePercent)
    ADD_CLASS_FIELD(double, dMeanCUDepthError, getMeanCUDepthError, setMeanCUDepthError)


};

#endif // COMSEQUENCE_H
