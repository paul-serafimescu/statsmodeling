#include <iostream>
#include <linalg.hpp>
#include <assert.h>

const int N = 2;
const int M = 5;
const int P = 5;

int main(void)
{
    auto A = matrix::from_random(N, N, 10);

    std::cout << A.to_string() << std::endl;
    std::cout << A.inverse().to_string() << std::endl;
    return EXIT_SUCCESS;
}
