#ifndef PARSERUTILITIES_H
#define PARSERUTILITIES_H

#include <QTextStream>
#include <QVector>
#include <QString>

class ParserUtilities
{
public:
    ParserUtilities();
    ~ParserUtilities();

    static void stream2IntAray(QTextStream* inputStream, QVector<int>* arr);
    static QString readNonemptyLine(QTextStream * inputStream);

    /// SINGLETON
    static ParserUtilities* getInstance();

private:

    static ParserUtilities* m_instance;
};

#endif // PARSERUTILITIES_H
