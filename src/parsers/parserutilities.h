#ifndef PARSERUTILITIES_H
#define PARSERUTILITIES_H

#include <QTextStream>

class ParserUtilities
{
public:
    ParserUtilities();
    ~ParserUtilities();

    static void stream2IntAray(QTextStream* inputStream, QVector<int>* arr);

    /// SINGLETON
    static ParserUtilities* getInstance();

private:

    static ParserUtilities* m_instance;
};

#endif // PARSERUTILITIES_H
