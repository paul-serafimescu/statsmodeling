#include <gtest/gtest.h>

#include "linalg.hpp"
#include "OLS.hpp"
#include "frame.hpp"

#include <filesystem>

using statsmodeling::OLS;

namespace fs = std::filesystem;

const fs::path CSV_PATH = fs::path("test/csv");

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

// ensure the csv data fit outputs and display string are correct
TEST(OLS, CorrectFitFromCsv) {
    fs::path csv_path = fs::current_path() / CSV_PATH / fs::path("ols_data.csv");
    auto df = frame::from_csv(csv_path.c_str());

    auto y = df["target"];
    auto X = df[{ "x1", "x2" }];

    auto reg = OLS(false);
    auto result = reg.fit(X, y);

    EXPECT_NEAR(result.get_param("B1"), 1, 1e-9);
    EXPECT_NEAR(result.get_param("B2"), 1, 1e-9);

    std::string correctOutput = "B0: NULL\nB1: 1\nB2: 1\n";

    EXPECT_EQ(correctOutput, result.display());
}

TEST(OLS, CorrectPlotFromCsv) {
    fs::path csv_path = fs::current_path() / CSV_PATH / fs::path("ols_data_1d.csv");
    auto df = frame::from_csv(csv_path.c_str());

    auto y = df["target"];
    auto X = df[{ "x1" }];

    auto reg = OLS();
    auto result = reg.fit(X, y);

    result.plot(); 
}
