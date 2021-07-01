#ifndef MATRIX
#define MATRIX

#include "IMatrix.h"


// This implementation of a Matrix stores all elements in a single buffer/array.
// First row is stored in  m_pElements [0, m_numberOfColumns - 1]
// Second row is stored in m_pElements [m_numberOfColumns, 2 * m_numberOfColumns - 1]
// and so on...
// This is utilized when searching for max product.
class Matrix: public IMatrix
{
public:
    Matrix();
    ~Matrix();

private:
    int alloc(size_t numberOfRows, size_t numberOfColumns);
    void free();
    int setElement(size_t rowIdx, size_t columnIdx, uint8_t element);
    int searchMaxProduct(MatrixProduct_t* pMaxProduct, size_t numberOfElements);
    MatrixProduct_t searchMaxProduct(MatrixProductType_e productType, size_t numberOfElements);

    bool hasDiagonals(size_t numberOfElements);

    size_t m_numberOfRows;
    size_t m_numberOfColumns;
    uint8_t *m_pElements;

};

#endif //MATRIX
         
