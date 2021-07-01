
#include <iostream>

#include "MatrixReader.h"
#include "MatrixParser.h"
#include "Matrix.h"
#include "ErrorCodes.h"

//-----------------------------------------------------------------------------
class MatrixSearchApp
{
public:
    void runApp(IMatrixReader *pMatrixReader, IMatrixParser *pMatrixParser, IMatrix *pMatrix);

};

//-----------------------------------------------------------------------------
int main()
{

    MatrixReader matrixReader;
    Matrix matrix;
    MatrixParser matrixParser;
    MatrixSearchApp matrixSearchApp;

    matrixSearchApp.runApp(&matrixReader, &matrixParser, &matrix);
    return 0;
}

//-----------------------------------------------------------------------------
void MatrixSearchApp::runApp(IMatrixReader* pMatrixReader, IMatrixParser* pMatrixParser, IMatrix* pMatrix)
{
    int ret;

    ret = pMatrixParser->parseMatrixString(pMatrix, pMatrixReader->getMatrixString());
    if (ret != E_SUCCESS)
    {
        std::cout << "Failed parsing matrix string.\n";
        return;
    }
    
    MatrixProduct_t maxProduct;

    ret = pMatrix->searchMaxProduct(&maxProduct, 4);
    if (ret != E_SUCCESS)
    {
        std::cout << "Failed finding max product.\n";
        return;
    }

    std::cout << "Max product is " << maxProduct.product << 
        " found in " << productTypeStr(maxProduct.productType) << 
        " starting at row = " << maxProduct.firstElementRowIdx <<
        " column = " << maxProduct.firstElementColumnIdx << 
        ".\n";
}

