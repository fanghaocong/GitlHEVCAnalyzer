#ifndef MERGEPARSER_H
#define MERGEPARSER_H

#include <QObject>
#include <QTextStream>
#include "model/common/comsequence.h"

class MergeParser : public QObject
{
    Q_OBJECT
public:
    explicit MergeParser(QObject *parent = 0);
    bool parseFile(QTextStream* pcInputStream, ComSequence* pcSequence);
protected:
    bool xReadMergeIndex(QTextStream* pcMergeIndexStream, ComCU* pcCU, ComSequence* pcSequence);
    bool xReadMergeIndex_HEVC(QTextStream* pcMergeIndexStream, ComCU* pcCU);
    bool xReadMergeIndex_VVC(QTextStream* pcMergeIndexStream, ComCU* pcCU);
signals:

public slots:

};

#endif // MERGEPARSER_H
