#ifndef BITSTREAMPARSER_H
#define BITSTREAMPARSER_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include "gitlmodual.h"
#include "model/common/comsequence.h"
class BitstreamParser : public QObject, public GitlModual
{
    Q_OBJECT
public:
    explicit BitstreamParser(QObject *parent = 0);
    ~BitstreamParser();
    bool parseFile(QString strDecoderFolder,
                   QString strDecoder,
                   int iDecoderVersion,
                   QString strBitstreamFilePath,
                   QString strTempOutputPath,
                   ComSequence* pcSequence);

    ADD_CLASS_FIELD_PRIVATE(QProcess, cDecoderProcess)  ///< HM Decoder Process
    ADD_CLASS_FIELD_PRIVATE(QFile, cStdOutputFile)      ///< HM Decoder Standard Output File
signals:

public slots:
    void displayDecoderOutput();                        ///< HM Decoder Output

};

#endif // BITSTREAMPARSER_H
