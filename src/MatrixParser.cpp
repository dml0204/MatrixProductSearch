
#include "MatrixParser.h"
#include "ErrorCodes.h"
#include <string.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------
static inline bool isDigit(char c)
{
    if ((c < '0') || (c > '9'))
    {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
int MatrixParser::parseMatrixString(IMatrix* pMatrix, const char* pMatrixString)
{
    m_pMatrix = pMatrix;
    m_pMatrixString = pMatrixString;

    int ret;

    if ((ret = checkFormat(pMatrixString)) != E_SUCCESS)
    {
        return ret;
    }
    return setMatrix();
}

//-----------------------------------------------------------------------------
int MatrixParser::checkFormat(const char* pMatrixString)
{
    m_nRows = 0;
    m_nColumns = 0;

    size_t matrixStrLen;

    matrixStrLen = strlen(pMatrixString);
    if (matrixStrLen < 2)
    {
        return E_FAIL;
    }

    size_t rowStrLen;
    const char *pNewline = strchr(pMatrixString, '\n');
    
    if (pNewline == NULL)
    {
        rowStrLen = matrixStrLen;
    }
    else
    {
        rowStrLen = pNewline - pMatrixString;
    }

    //Check valid number of characters for one line and 
    //that total string length is divisable with length of line
    if (((rowStrLen + 1) % 3) || ((matrixStrLen + 1) % (rowStrLen + 1)))
    {
        return E_FAIL;
    }

    m_nRows = (matrixStrLen + 1) / (rowStrLen + 1);
    m_nColumns = (rowStrLen + 1) / 3;

    //Check for valid character for complete string..
    for (size_t rowIdx = 0; rowIdx < m_nRows; rowIdx++)
    {
        size_t rowStartIdx = rowIdx * 3 * m_nColumns;
        for (size_t columnIdx = 0; columnIdx < m_nColumns; columnIdx++)
        {
            size_t firstDigitIdx = rowStartIdx + columnIdx * 3;
            if (!isDigit(m_pMatrixString[firstDigitIdx]) || (!isDigit(m_pMatrixString[firstDigitIdx + 1])))
            {
                return E_INVALIDARG;
            }
            if (columnIdx < (m_nColumns - 1))
            {
                if (m_pMatrixString[firstDigitIdx + 2] != ' ')
                {
                    return E_INVALIDARG;
                }
            }
        }
        if ((rowIdx < (m_nRows - 1)) && (m_nRows > 1))
        {
            size_t newLineIdx = (rowIdx + 1) * 3 * m_nColumns - 1;
            if (m_pMatrixString[newLineIdx] != '\n')
            {
                return E_INVALIDARG;
            }
        }
    }
    return E_SUCCESS;
}

//-----------------------------------------------------------------------------
int MatrixParser::setMatrix()
{
    int ret;

    if ((ret = m_pMatrix->alloc(m_nRows, m_nColumns)) != E_SUCCESS)
    {
        return ret;
    }
     
    for (size_t rowIdx = 0; rowIdx < m_nRows; rowIdx++)
    {
        size_t rowStartIdx = rowIdx * 3 * m_nColumns;
        for (size_t columnIdx = 0; columnIdx < m_nColumns; columnIdx++)
        {
            char twoDigitStr[3] = "";
            size_t firstDigitIdx = rowStartIdx + columnIdx * 3;
            twoDigitStr[0] = m_pMatrixString[firstDigitIdx];
            twoDigitStr[1] = m_pMatrixString[firstDigitIdx + 1];
            m_pMatrix->setElement(rowIdx, columnIdx, atoi(twoDigitStr));
        }
    }
    return E_SUCCESS;
}

