
#include "Matrix.h"
#include "ErrorCodes.h"
#include "Assert.h"
#include <stdint.h>
#include <memory.h>

static inline uint64_t arrayProduct(uint8_t* pArray, int64_t step, size_t numberOfElements);
static inline void updateMaxProduct(MatrixProduct_t* pMaxProduct, MatrixProduct_t product);

//-----------------------------------------------------------------------------
Matrix::Matrix():
    m_numberOfRows(0),
    m_numberOfColumns(0),
    m_pElements(NULL)
{

}

//-----------------------------------------------------------------------------
Matrix::~Matrix()
{
    free();
}

//-----------------------------------------------------------------------------
int Matrix::alloc(size_t numberOfRows, size_t numberOfColumns)
{
    free();

    size_t numberOfElements;

    //todo: currently no sanity check on size...:
    numberOfElements = numberOfRows * numberOfColumns;
    m_pElements = new uint8_t[numberOfElements];
    if (m_pElements == NULL)
    {
        return E_OUTOFMEMORY;
    }

    memset((void *)m_pElements, 0, numberOfElements * sizeof(uint8_t));
    m_numberOfRows = numberOfRows;
    m_numberOfColumns = numberOfColumns;

    return E_SUCCESS;
}

//-----------------------------------------------------------------------------
void Matrix::free()
{
    if (m_pElements != NULL)
    {
        delete m_pElements;
        m_pElements = NULL;
    }
    m_numberOfRows = 0;
    m_numberOfColumns = 0;
}

//-----------------------------------------------------------------------------
int Matrix::setElement(size_t rowIdx, size_t columnIdx, uint8_t element)
{
    if (m_pElements == NULL)
    {
        return E_NODATA;
    }
    if ((rowIdx >= m_numberOfRows) || (columnIdx >= m_numberOfColumns))
    {
        return E_INVALIDARG;
    }
    m_pElements[rowIdx * m_numberOfColumns + columnIdx] = element;
    return E_SUCCESS;
}

//-----------------------------------------------------------------------------
int Matrix::searchMaxProduct(MatrixProduct_t* pMaxProduct, size_t numberOfElements)
{
    if (m_pElements == NULL)
    {
        return E_NODATA;
    }

    if ((m_numberOfRows < numberOfElements) && (m_numberOfColumns < numberOfElements))
    { //No product exists..
        return E_INVALIDARG;
    }
    
    //Make sure it is updated:
    pMaxProduct->productType = eMATRIX_PRODUCT_INVALID;

    updateMaxProduct(pMaxProduct, searchMaxProduct(eMATRIX_PRODUCT_ROW, numberOfElements));
    updateMaxProduct(pMaxProduct, searchMaxProduct(eMATRIX_PRODUCT_COLUMN, numberOfElements));
    updateMaxProduct(pMaxProduct, searchMaxProduct(eMATRIX_PRODUCT_DIAGONAL, numberOfElements));
    updateMaxProduct(pMaxProduct, searchMaxProduct(eMATRIX_PRODUCT_ANTIDIAGONAL, numberOfElements));

    return E_SUCCESS;
}

//-----------------------------------------------------------------------------
MatrixProduct_t Matrix::searchMaxProduct(MatrixProductType_e productType, size_t numberOfElements)
{
    MatrixProduct_t maxProduct;

    int64_t step;
    size_t  numberOfFirstColumns;
    size_t  firstFirstRowIdx;
    size_t  lastFirstRowIdx;

    //Check if a product exist for this type and set search parameters:
    switch (productType)
    {
        case eMATRIX_PRODUCT_ROW:
            if (m_numberOfColumns < numberOfElements)
            {
                return maxProduct;
            }
            step = 1;
            numberOfFirstColumns = m_numberOfColumns - numberOfElements;
            firstFirstRowIdx = 0;
            lastFirstRowIdx = m_numberOfRows - 1;
            break;

        case eMATRIX_PRODUCT_COLUMN:
            if (m_numberOfRows < numberOfElements)
            {
                return maxProduct;
            }
            step = m_numberOfColumns;
            numberOfFirstColumns = m_numberOfColumns;
            firstFirstRowIdx = 0;
            lastFirstRowIdx = m_numberOfRows - numberOfElements - 1;
            break;

        case eMATRIX_PRODUCT_DIAGONAL:
            if (!hasDiagonals(numberOfElements))
            {
                return maxProduct;
            }
            step = m_numberOfColumns + 1;
            numberOfFirstColumns = m_numberOfColumns - numberOfElements;
            firstFirstRowIdx = 0;
            lastFirstRowIdx = m_numberOfRows - numberOfElements - 1;
            break;

        case eMATRIX_PRODUCT_ANTIDIAGONAL:
            if (!hasDiagonals(numberOfElements))
            {
                return maxProduct;
            }
            //First element in product is leftmost/bottom element.
            //This gives a negative step for element in row above...
            step = -((int64_t)m_numberOfColumns - 1);
            numberOfFirstColumns = m_numberOfColumns - numberOfElements;
            firstFirstRowIdx = numberOfElements - 1;
            lastFirstRowIdx = m_numberOfRows - 1;
            break;

        default:
            ASSERT(false);
            return maxProduct;
    }
 
    MatrixProduct_t p;

    maxProduct.productType = p.productType = productType;

    for (p.firstElementColumnIdx = 0; p.firstElementColumnIdx < numberOfFirstColumns; p.firstElementColumnIdx++)
    {
        for (p.firstElementRowIdx = firstFirstRowIdx; p.firstElementRowIdx <= lastFirstRowIdx; p.firstElementRowIdx++)
        {
            p.product = arrayProduct(&m_pElements[p.firstElementRowIdx * m_numberOfColumns + p.firstElementColumnIdx], step, numberOfElements);
            updateMaxProduct(&maxProduct, p);
        }
    }
    return maxProduct;
}

//-----------------------------------------------------------------------------
bool Matrix::hasDiagonals(size_t numberOfElements)
{
    if ((m_numberOfRows < numberOfElements) || (m_numberOfColumns < numberOfElements))
    {
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
static inline uint64_t arrayProduct(uint8_t* pArray, int64_t step, size_t numberOfElements)
{
    uint64_t product;

    product = (uint64_t)(*pArray);
    for (size_t elementIdx = 1; elementIdx < numberOfElements; elementIdx++)
    {
        pArray += step;
        product *= (uint64_t)(*pArray);
    }
    return product;
}

//-----------------------------------------------------------------------------
static inline void updateMaxProduct(MatrixProduct_t* pMaxProduct, MatrixProduct_t product)
{
    if ((pMaxProduct->productType == eMATRIX_PRODUCT_INVALID) ||
        (product.product > pMaxProduct->product))
    {
        *pMaxProduct = product;
    }
}

