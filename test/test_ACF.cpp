#include <gtest/gtest.h>

#include "linalg.hpp"
#include "ACF.hpp"
#include "frame.hpp"

#include <filesystem>

using statsmodeling::ACF;

namespace fs = std::filesystem;

const fs::path CSV_PATH = fs::path("test/csv");

// ensure the csv data fit outputs and display string are correct
TEST(OLS, CorrectACFFromCsv) {
    fs::path csv_path = fs::current_path() / CSV_PATH / fs::path("timeseries.csv");
    auto df = frame::from_csv(csv_path.c_str());

    auto y = df["gdp"];

    auto acf = ACF(y);
    auto autocorrelations = acf.fit();

    // make sure abs val of autocorrelations are less or equal to 1
    for (const auto& p : autocorrelations[0UL]) {
        EXPECT_LE(std::abs(p), 1);
    }

    // make sure dimensions are correct
    EXPECT_EQ(y.T().get_dim(), autocorrelations.get_dim());
}
