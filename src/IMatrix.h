#ifndef IMATRIX
#define IMATRIX

#include <stdint.h>

typedef enum
{
    eMATRIX_PRODUCT_ROW,
    eMATRIX_PRODUCT_COLUMN,
    eMATRIX_PRODUCT_DIAGONAL,
    eMATRIX_PRODUCT_ANTIDIAGONAL,
    eMATRIX_PRODUCT_INVALID = -1
}MatrixProductType_e;

//First element in product is leftmost element, or topmost in case of eMATRIX_PRODUCT_COLUMN...
typedef struct MatrixProduct_tTag
{
    MatrixProductType_e productType;
    size_t              firstElementRowIdx;
    size_t              firstElementColumnIdx;
    uint64_t            product;

    MatrixProduct_tTag()
    {
        productType = eMATRIX_PRODUCT_INVALID;
        firstElementRowIdx = 0;
        firstElementColumnIdx = 0;
        product = 0;
    };
}MatrixProduct_t;


static const char* productTypeStr(MatrixProductType_e productType)
{
    switch (productType)
    {
    case eMATRIX_PRODUCT_ROW:
        return "row";

    case eMATRIX_PRODUCT_COLUMN:
        return "column";

    case eMATRIX_PRODUCT_DIAGONAL:
        return "diagonal";

    case eMATRIX_PRODUCT_ANTIDIAGONAL:
        return "anti diagonal";

    case eMATRIX_PRODUCT_INVALID:
        return "invalid";

    default:
        return "unhandled";
    }
}

class IMatrix
{
public:
    virtual int alloc(size_t numberOfRows, size_t numberOfColumns) = 0;
    virtual int setElement(size_t rowIdx, size_t columnIdx, uint8_t element) = 0;
    virtual int searchMaxProduct(MatrixProduct_t *pProduct, size_t numberOfElements) = 0;

protected:
    ~IMatrix() {};

};

#endif //IMATRIX
         
