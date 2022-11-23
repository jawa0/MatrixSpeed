#include <array>
#include <chrono>
#include <iostream>


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
                m_elements[i] = static_cast<ElementT>(rand()) / RAND_MAX * 2.0f - 1.0f;
            }
        }

        // Matrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1>
        template <size_t NKernelRows, size_t NKernelColumns>
        Matrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1>
        convolve(
            const Matrix<ElementT, NKernelRows, NKernelColumns>& kernel,
            Matrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1>& result) const
        {
            using namespace std;

            // Keep the kernel within bounds of the image matrix. This means that the resulting image will be smaller.
            // Matrix<ElementT, NRows - NKernelRows + 1, NColumns - NKernelColumns + 1> result;

            // cout << kernel.NumRows << " " << kernel.NumColumns << endl;
            // cout << result.NumRows << " " << result.NumColumns << endl;

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


Matrix<float, 1920, 1080>   image;
Matrix<float, 25, 25>       kernel;
Matrix<
    float, 
    image.NumRows - kernel.NumRows + 1, 
    image.NumColumns - kernel.NumColumns + 1> 
result;  // @note - dislike having to specify the template arguments in this way.


int main()
{
    using namespace std;


    cout << image.NumRows << " " << image.NumColumns << endl;
    cout << kernel.NumRows << " " << kernel.NumColumns << endl;

    auto start = chrono::high_resolution_clock::now();
    {
        image.randomInit();
        kernel.randomInit();
    }
    auto end = chrono::high_resolution_clock::now();
    auto elapsedUSec = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Matrix initialization time: " << elapsedUSec << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    
    image.convolve(kernel, result);

    end = chrono::high_resolution_clock::now();
    elapsedUSec = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // prevent result from being optimized away
    for (size_t row = 0; row < result.NumRows; ++row)
    {
        for (size_t column = 0; column < result.NumColumns; ++column)
        {
            cout << result(row, column) << " ";
            break;
        }
        cout << endl;
        break;
    }

    cout << "Convolution time: " << elapsedUSec << " ms" << endl;
    return 0;
}
