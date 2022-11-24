#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "jmatrix.hpp"


auto constexpr FHD_WIDTH_PIXELS = 1920;
auto constexpr FHD_HEIGHT_PIXELS = 1080;

Matrix<float> image(FHD_WIDTH_PIXELS, FHD_HEIGHT_PIXELS);
std::vector< size_t > kernelSizes = { 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31};
std::vector< Matrix<float> > kernels;
std::vector< Matrix<float> > results;

int main()
{
    using namespace std;

    // Seed the random number generator using a different seed each time the program is run.
    // This prevents the generated (pseudo-) random numbers from being the same each time.

    auto seed = chrono::system_clock::now().time_since_epoch().count();
    srand(seed);

    cout << "#-----------------------------------------------------------------------------" << endl;
    cout << "# Matrix 2D Convolution Speed Test" << endl;
    cout << "#-----------------------------------------------------------------------------" << endl;
    cout << "# Image size: " << FHD_WIDTH_PIXELS << " x " << FHD_HEIGHT_PIXELS << endl;
    cout << "# Naive C++ implementation:" << endl;
    cout << "#  * CPU-only" << endl;
    cout << "#  * single-threaded" << endl;
    cout << "#  * image and kernel stored column-major" << endl << "#" << endl;
    cout << "# Seed value for random number generator: " << seed << endl;
    cout << "#-----------------------------------------------------------------------------" << endl;

    // Prevent calculation work from being optimized away because no result is used.
    ofstream antiOptimizierOutput("throwaway_results_to_prevent_optimizing_away_calculation.txt");

    image.randomInit();
    cout << "Kernel Size,Convolution Time (milliseconds)" << endl;
    for (auto kernelSize : kernelSizes)
    {
        auto k = Matrix<float>(kernelSize, kernelSize);
        k.randomInit();
        kernels.push_back(k);

        // Convolution with no padding and stride 1. This means that the resulting 
        // image will be smaller.
        Matrix<float> result(image.nrows() - k.nrows() + 1, image.ncols() - k.ncols() + 1);

        auto start = chrono::high_resolution_clock::now();
        image.convolve(k, result);
        auto end = chrono::high_resolution_clock::now();
        auto elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        results.push_back(result);

        cout << "\"(" << k.nrows() << ", " << k.ncols() << ")\"," << elapsedMilliseconds << endl;

        // Prevent calculation work from being optimized away because no result is used.
        // Randomly choose an element of the result matrix to print out. This means that
        // the compiler can't know which element will be printed, so it can't optimize away
        // the entire calculation.

        auto done = false;
        auto value = 0.f;
        for (size_t row = 0; !done && row < result.nrows(); ++row)
        {
            for (size_t column = 0; !done && column < result.ncols(); ++column)
            {
                if ((rand() / (float) RAND_MAX) < 0.01f)
                {
                    value = result(row, column);
                    done = true;
                }
            }
        }
        antiOptimizierOutput << value << endl;
    }

    return 0;
}
