#include <iostream>
#include <assert.h>

#include "linalg.hpp"
#include "OLS.hpp"

using statsmodeling::OLS;

const int N = 2;
const int M = 5;
const int P = 5;

int main(void)
{
    auto y = matrix::from({
        { 3 },
        { 6 },
        { 9 },
        { 12 }
    });

    auto X = matrix::from({
        { 0 },
        { 1 },
        { 2 },
        { 3 }
    });

    auto reg = OLS(false);
    auto result = reg.fit(X, y);

    std::cout << result.display() << std::endl;

    return EXIT_SUCCESS;
}
