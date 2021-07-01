#ifndef MATRIX_PARSER
#define MATRIX_PARSER

#include "IMatrixParser.h"

class MatrixParser: public IMatrixParser
{
private:
    int parseMatrixString(IMatrix* pMatrix, const char* pMatrixString);
    int checkFormat(const char* pMatrixString);
    int setMatrix();

    IMatrix* m_pMatrix;
    const char* m_pMatrixString;

    size_t m_nRows;
    size_t m_nColumns;
};

#endif //MATRIX_PARSER

