#ifndef IMATRIX_READER
#define IMATRIX_READER

class IMatrixReader
{
public:
    virtual const char* getMatrixString() = 0;

protected:
    ~IMatrixReader() {};
};

#endif //IMATRIX_READER

