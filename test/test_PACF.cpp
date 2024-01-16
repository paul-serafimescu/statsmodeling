#include <gtest/gtest.h>

#include "linalg.hpp"
#include "PACF.hpp"
#include "frame.hpp"

#include <filesystem>

using statsmodeling::PACF;

namespace fs = std::filesystem;

const fs::path CSV_PATH = fs::path("test/csv");

// ensure the csv data fit outputs and display string are correct
TEST(OLS, CorrectPACFFromCsv) {
    fs::path csv_path = fs::current_path() / CSV_PATH / fs::path("timeseries.csv");
    auto df = frame::from_csv(csv_path.c_str());

    auto y = df["inflation"];

    auto pacf = PACF(y);
    auto partial_autocorrelations = pacf.fit();
    // come up with test cases for this somehow idk
}
