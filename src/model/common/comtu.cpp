#include "comtu.h"

ComTU::ComTU()
{
}


bool ComTU::isValid(int cuLeft, int cuTop, int cuRight, int cuBottom)
{
    if ( m_iX < cuLeft || m_iX > cuRight )
    {
        return false;
    }
    if ( m_iY < cuTop || m_iY > cuBottom )
    {
        return false;
    }

    int tuRight = m_iX + m_iWidth - 1;
    if (tuRight < m_iX || tuRight > cuRight )
    {
        return false;
    }

    int tuBottom = m_iY + m_iHeight - 1;
    if (tuBottom < m_iY || tuBottom > cuBottom )
    {
        return false;
    }

    return true;
}
