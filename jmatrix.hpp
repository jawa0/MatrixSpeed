#include <array>
#include <memory>
#include <vector>


// A matrix with compile-time fixed size, stored in column-major order.
template <typename ElementT, size_t NRows, size_t NColumns>
class StaticMatrix
{
    public:
        static constexpr size_t NumRows = NRows;
        static constexpr size_t NumColumns = NColumns;

        StaticMatrix() = default;
        StaticMatrix(const StaticMatrix&) = default;
        StaticMatrix(StaticMatrix&&) = default;
        StaticMatrix& operator=(const StaticMatrix&) = default;
        StaticMatrix& operator=(StaticMatrix&&) = default;

        ElementT& operator()(size_t row, size_t column)
        {
            return m_elements[row * NColumns + column];
        }

        const ElementT& operator()(size_t row, size_t column) const
        {
            return m_elements[row * NColumns + column];
        }

        void randomInit()
        {
            for (size_t i = 0; i < NRows * NColumns; ++i)
            {
                // Generate a random number between -1.0 and 1.0.
                m_elements[i] = (rand() / (ElementT) RAND_MAX) * 2.0f - 1.0f;
            }
        }

        // Convolution with no padding and stride 1.
        // Keep the kernel within bounds of the image matrix. This means that the resulting image will be smaller.
        
        template <size_t NKernelRows, size_t NKernelColumns>
        void convolve(
            const StaticMatrix<ElementT, NKernelRows, NKernelColumns>& kernel,
            StaticMatrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1>& result) const
        {
            for (size_t iFirstRow = 0; iFirstRow < result.NumRows; ++iFirstRow)
            {
                for (size_t iFirstColumn = 0; iFirstColumn < result.NumColumns; ++iFirstColumn)
                {
                    result(iFirstRow, iFirstColumn) = 0.0f;

                    for (size_t r = 0; r < kernel.NumRows; ++r)
                    {
                        for (auto c = 0; c < NKernelColumns; ++c)
                        {
                            result(iFirstRow, iFirstColumn) += (*this)(iFirstRow + r, iFirstColumn + c) * kernel(r, c);
                        }
                    }
                }
            }
        }
    private:
        std::array<ElementT, NRows * NColumns> m_elements;
};


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

        Matrix(const Matrix&) = default;
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