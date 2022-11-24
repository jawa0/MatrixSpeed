#include <array>
#include <memory>
#include <vector>


// A matrix with runtime-time initialized size, stored in column-major order.
template <typename ElementT>
class Matrix
{
    public:
        Matrix(size_t numRows, size_t numColumns)
        {
            m_numRows = numRows;
            m_numColumns = numColumns;
            m_elementsPtr = std::make_unique< std::vector<ElementT> >(m_numColumns * m_numRows);
        }

        Matrix(const Matrix& other)
        {
            m_numRows = other.m_numRows;
            m_numColumns = other.m_numColumns;
            m_elementsPtr = std::make_unique< std::vector<ElementT> >(m_numColumns * m_numRows);
            std::copy(other.m_elementsPtr->begin(), other.m_elementsPtr->end(), m_elementsPtr->begin());
        }

        Matrix(Matrix&&) = default;
        Matrix& operator=(const Matrix&) = default;
        Matrix& operator=(Matrix&&) = default;

        size_t inline nrows() const { return m_numRows; }

        size_t inline ncols() const { return m_numColumns; }

        ElementT& operator()(size_t row, size_t column)
        {
            return (*m_elementsPtr)[row * ncols() + column];
        }

        const ElementT& operator()(size_t row, size_t column) const
        {
            return (*m_elementsPtr)[row * ncols() + column];
        }

        void randomInit()
        {
            for (size_t i = 0; i < nrows() * ncols(); ++i)
            {
                // Generate a random number between -1.0 and 1.0.
                (*m_elementsPtr)[i] = (rand() / (ElementT) RAND_MAX) * 2.0f - 1.0f;
            }
        }

        // Convolution with no padding and stride 1.
        // Keep the kernel within bounds of the image matrix. This means that the resulting image will be smaller.
        
        void convolve(const Matrix<ElementT>& kernel, Matrix<ElementT>& result) const
        {
            for (size_t iFirstRow = 0; iFirstRow < result.nrows(); ++iFirstRow)
            {
                for (size_t iFirstColumn = 0; iFirstColumn < result.ncols(); ++iFirstColumn)
                {
                    result(iFirstRow, iFirstColumn) = 0.0f;

                    for (size_t r = 0; r < kernel.nrows(); ++r)
                    {
                        for (auto c = 0; c < kernel.ncols(); ++c)
                        {
                            result(iFirstRow, iFirstColumn) += (*this)(iFirstRow + r, iFirstColumn + c) * kernel(r, c);
                        }
                    }
                }
            }
        }
    private:
        size_t m_numRows;
        size_t m_numColumns;
        std::unique_ptr< std::vector<ElementT> > m_elementsPtr;
};