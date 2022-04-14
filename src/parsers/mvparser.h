#ifndef MVPARSER_H
#define MVPARSER_H

#include <QObject>
#include <QTextStream>
#include "model/common/comsequence.h"

class MVParser : public QObject
{
    Q_OBJECT
public:
    explicit MVParser(QObject *parent = 0);
    bool parseFile(QTextStream* pcInputStream, ComSequence* pcSequence);
protected:
    bool xReadMV(QTextStream* pcCUInfoStream, ComCU* pcCU, ComSequence* pcSequence);
    bool xReadMV_HEVC(QTextStream* pcCUInfoStream, ComCU* pcCU);
    bool xReadMV_VVC(QTextStream* pcCUInfoStream, ComCU* pcCU);
signals:

public slots:

};

#endif // MVPARSER_H
