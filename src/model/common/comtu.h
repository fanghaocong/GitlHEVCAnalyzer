#ifndef COMTU_H
#define COMTU_H
#include "gitldef.h"
#include <QVector>
class ComTU
{
public:
    ComTU();
    bool isValid(int cuLeft, int cuTop, int cuRight, int cuBottom);

    ADD_CLASS_FIELD(QVector<ComTU*>, apcTUs, getTUs, setTUs)
    ADD_CLASS_FIELD(int, iX, getX, setX)                                        ///< X Position in frame
    ADD_CLASS_FIELD(int, iY, getY, setY)                                        ///< Y Position in frame
    ADD_CLASS_FIELD(int, iWidth, getWidth, setWidth)                            ///< CU Width
    ADD_CLASS_FIELD(int, iHeight, getHeight, setHeight)                         ///< CU Height


};

#endif // COMTU_H
