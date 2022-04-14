#ifndef PREDPARSER_H
#define PREDPARSER_H

#include <QObject>
#include <QTextStream>
#include "model/common/comsequence.h"

class PredParser : public QObject
{
    Q_OBJECT
public:
    explicit PredParser(QObject *parent = 0);
    bool parseFile(QTextStream* pcInputStream, ComSequence* pcSequence);
protected:
    bool xReadPredMode(QTextStream* pcPredInfoStream, ComCU* pcCU, ComSequence* pcSequence);
    bool xReadPredMode_HEVC(QTextStream* pcPredInfoStream, ComCU* pcCU);
    bool xReadPredMode_VVC(QTextStream* pcPredInfoStream, ComCU* pcCU);
signals:

public slots:

};

#endif // PREDPARSER_H
