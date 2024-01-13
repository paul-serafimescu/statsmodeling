#include <iostream>
#include <assert.h>

#include "linalg.hpp"
#include "OLS.hpp"
#include "frame.hpp"

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
        { 1, 2 },
        { 2, 4 },
        { 4, 5 },
        { 2, 10 }
    });

    try {

        auto df = frame::from_csv("data.csv");
        auto y = df["target"];
        auto X = df[{ "column1", "column2" }];

        std::cout << y.to_string() << std::endl;
        std::cout << X.to_string() << std::endl;
        std::cout << df.display() << std::endl;

        auto reg = OLS(true);
        auto result = reg.fit(X, y);

        std::cout << result.display() << std::endl;
    } catch (const char *reason) {
        std::cerr << reason << std::endl;
    }

    return EXIT_SUCCESS;
}
