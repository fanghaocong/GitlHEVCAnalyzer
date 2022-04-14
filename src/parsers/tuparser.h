#ifndef TUPARSER_H
#define TUPARSER_H
#include <QObject>
#include <QTextStream>
#include "model/common/comsequence.h"

class TUParser : public QObject
{
    Q_OBJECT
public:
    explicit TUParser(QObject *parent = 0);
    bool parseFile(QTextStream* pcInputStream, ComSequence* pcSequence);
protected:
    bool xReadTU(QTextStream* pcCUInfoStream, ComCU* pcTU, ComSequence* pcSequence);
    bool xReadTU_HEVC(QTextStream* pcCUInfoStream, ComCU* pcTU);
    bool xReadTU_VVC(QTextStream* pcCUInfoStream, ComCU* pcTU);
    bool xReadTUHelper(QTextStream* pcCUInfoStream, ComTU *pcTU);
};

#endif // TUPARSER_H
