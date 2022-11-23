#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include "jmatrix.hpp"


auto constexpr FHD_WIDTH_PIXELS = 1920;
auto constexpr FHD_HEIGHT_PIXELS = 1080;

Matrix<float, FHD_WIDTH_PIXELS, FHD_HEIGHT_PIXELS> image;
Matrix<float, 5, 5> kernel;

// Convolution with no padding and stride 1. This means that the resulting 
// image will be smaller.

Matrix<
    float, 
    image.NumRows - kernel.NumRows + 1, 
    image.NumColumns - kernel.NumColumns + 1> 
result;


int main()
{
    using namespace std;

    srand(time(nullptr));

    cout << image.NumRows << " " << image.NumColumns << endl;
    cout << kernel.NumRows << " " << kernel.NumColumns << endl;

    auto start = chrono::high_resolution_clock::now();
    {
        image.randomInit();
        kernel.randomInit();
    }
    auto end = chrono::high_resolution_clock::now();
    auto elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Matrix initialization time: " << elapsedMilliseconds << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    
    image.convolve(kernel, result);

    end = chrono::high_resolution_clock::now();
    elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Prevent calculation work from being optimized away because no result is used.
    // Randomly choose an element of the result matrix to print out. This means that
    // the compiler can't know which element will be printed, so it can't optimize away
    // the entire calculation.

    auto done = false;
    auto value = 0.f;
    for (size_t row = 0; !done && row < result.NumRows; ++row)
    {
        for (size_t column = 0; !done && column < result.NumColumns; ++column)
        {
            if ((rand() / (float) RAND_MAX) < 0.01f)
            {
                value = result(row, column);
                done = true;
            }
        }
    }

    cout << "Convolution time: " << elapsedMilliseconds << " ms" << endl;
    cout << value << endl;
    return 0;
}
