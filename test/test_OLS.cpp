#include <gtest/gtest.h>
#include <iostream>

#include "linalg.hpp"
#include "OLS.hpp"

using statsmodeling::OLS;

// ensure OLS slope estimates with constant term are correct
TEST(OLS, CorrectFitWithConstant) {
    auto y = matrix::from({
        { 4 },
        { 7 },
        { 10 },
        { 13 }
    });

    auto X = matrix::from({
        { 1, 2 },
        { 2, 4 },
        { 4, 5 },
        { 2, 10 }
    });

    auto reg = OLS();
    auto result = reg.fit(X, y);

    EXPECT_NEAR(result.get_param("B1"), 1, 1e-9);
    EXPECT_NEAR(result.get_param("B2"), 1, 1e-9);
}

// ensure OLS slope estimates without constant term are correct
TEST(OLS, CorrectFitWithoutConstant) {
    auto y = matrix::from({
        { 4 },
        { 7 },
        { 10 },
        { 13 }
    });

    auto X = matrix::from({
        { 1, 2 },
        { 2, 4 },
        { 4, 5 },
        { 2, 10 }
    });

    auto reg = OLS(false);
    auto result = reg.fit(X, y);

    EXPECT_NEAR(result.get_param("B1"), 1.2267, 1e-4);
    EXPECT_NEAR(result.get_param("B2"), 1.0667, 1e-4);
}

// ensure the display string is correct
TEST(OLS, CorrectFitResultDisplay) {
    auto y = matrix::from({
        { 4 },
        { 7 },
        { 10 },
        { 13 }
    });

    auto X = matrix::from({
        { 1, 2 },
        { 2, 4 },
        { 4, 5 },
        { 2, 10 }
    });

    auto reg = OLS(false);
    auto result = reg.fit(X, y);

    std::string correctOutput = "B0: NULL\nB1: 1.22667\nB2: 1.06667\n";

    EXPECT_EQ(correctOutput, result.display());
}
