#include <array>
#include <chrono>
#include <iostream>
#include <vector>

#include "jmatrix.hpp"


auto constexpr FHD_WIDTH_PIXELS = 1920;
auto constexpr FHD_HEIGHT_PIXELS = 1080;

StaticMatrix<float, FHD_WIDTH_PIXELS, FHD_HEIGHT_PIXELS> g_image;
Matrix<float> g_dimage(FHD_WIDTH_PIXELS, FHD_HEIGHT_PIXELS);

StaticMatrix<float, 25, 25> skernel;
Matrix<float> dkernel(25, 25);

// Convolution with no padding and stride 1. This means that the resulting 
// image will be smaller.

StaticMatrix<
    float, 
    g_image.NumRows - skernel.NumRows + 1, 
    g_image.NumColumns - skernel.NumColumns + 1> 
result;



int main()
{
    Matrix<float> dresult(g_dimage.nrows() - dkernel.nrows() + 1, g_dimage.ncols() - dkernel.ncols() + 1);

    using namespace std;
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "## Matrix 2D Convolution Speed Test ##" << endl;
    cout << "--------------------------------------------------------------------------------" << endl;
    
    // vector< StaticMatrix< float, 5, 5 > > kernels;

    // Seed the random number generator using a different seed each time the program is run.
    // This prevents the generated (pseudo-) random numbers from being the same each time.

    auto seed = chrono::system_clock::now().time_since_epoch().count();
    cout << "Seed value for random number generator: " << seed << endl << endl;
    srand(seed);

    auto start = chrono::high_resolution_clock::now();
    {
        g_image.randomInit();
        skernel.randomInit();
        dkernel.randomInit();
    }
    auto end = chrono::high_resolution_clock::now();
    auto elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Matrix initialization time: " << elapsedMilliseconds << " ms" << endl;

    start = chrono::high_resolution_clock::now();
    g_image.convolve(skernel, result);
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

    cout << "Static Convolution time: " << elapsedMilliseconds << " ms" << endl;
    cout << value << endl;

    start = chrono::high_resolution_clock::now();
    g_dimage.convolve(dkernel, dresult);
    end = chrono::high_resolution_clock::now();
    elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    // Prevent calculation work from being optimized away because no result is used.
    // Randomly choose an element of the result matrix to print out. This means that
    // the compiler can't know which element will be printed, so it can't optimize away
    // the entire calculation.

    done = false;
    value = 0.f;
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

    cout << "Dynamic Convolution time: " << elapsedMilliseconds << " ms" << endl;
    cout << value << endl;

    return 0;
}
