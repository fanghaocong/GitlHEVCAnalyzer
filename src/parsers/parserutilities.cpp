#include "parserutilities.h"

ParserUtilities* ParserUtilities::m_instance = NULL;

ParserUtilities::ParserUtilities()
{

}


ParserUtilities::~ParserUtilities()
{

}

ParserUtilities* ParserUtilities::getInstance()
{
    if ( m_instance == NULL )
    {
        m_instance = new ParserUtilities();
    }
    return m_instance;
}

void ParserUtilities::stream2IntAray(QTextStream* inputStream, QVector<int>* arr)
{
    while ( !(inputStream->atEnd()) )
    {
        int num;
        *inputStream >> num;
        arr->push_back(num);
    }
}

QString ParserUtilities::readNonemptyLine(QTextStream * inputStream)
{
    QString strOneLine;
    do {
        strOneLine = inputStream->readLine();
    } while (strOneLine.length() <= 0 && !inputStream->atEnd());
    return strOneLine;
}
