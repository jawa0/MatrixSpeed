#include <array>


// This is a matrix stored in column-major order inside a std::array.
template <typename ElementT, size_t NRows, size_t NColumns>
class Matrix
{
    public:
        static constexpr size_t NumRows = NRows;
        static constexpr size_t NumColumns = NColumns;

        Matrix() = default;
        Matrix(const Matrix&) = default;
        Matrix(Matrix&&) = default;
        Matrix& operator=(const Matrix&) = default;
        Matrix& operator=(Matrix&&) = default;

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
        Matrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1>
        convolve(
            const Matrix<ElementT, NKernelRows, NKernelColumns>& kernel,
            Matrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1>& result) const
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

            return result;
        }
    private:
        std::array<ElementT, NRows * NColumns> m_elements;
};
