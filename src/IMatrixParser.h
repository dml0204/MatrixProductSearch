#ifndef IMATRIX_PARSER
#define IMATRIX_PARSER

#include "IMatrix.h"


class IMatrixParser
{
public:
    virtual int parseMatrixString(IMatrix* pMatrix, const char* pMatrixString) = 0;

protected:
    ~IMatrixParser() {};
};

#endif //IMATRIX_PARSER

