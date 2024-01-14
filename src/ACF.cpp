/**
 * @file ACF.cpp
 * @author Paul Serafimescu (pserafimescu9@ucla.edu)
 * @brief AutoCorrelation Function Implementation
 * @version 0.1
 * @date 2024-01-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "ACF.hpp"

#include <iostream>

namespace statsmodeling
{
    ACF::ACF(std::initializer_list<double> timeseries)
    {
        std::vector<double> ts(timeseries);
        T = timeseries.size();
        u = statsmodeling::utils::mean(ts);
        this->timeseries = ts;
    }

    ACF::ACF(std::vector<double> timeseries)
    {
        T = timeseries.size();
        u = statsmodeling::utils::mean(timeseries);
        this->timeseries = timeseries;
    }

    ACF::ACF(const Matrix& timeseriesM)
    {
        auto dim = timeseriesM.get_dim();

        if (dim.cols != 1) {
            std::stringstream ss;
            ss << "detected " << dim.cols << " series, should have only 1";
            throw ss.str().c_str();
        }

        auto timeseries = timeseriesM.T()[0UL];

        T = timeseries.size();
        u = statsmodeling::utils::mean(timeseries);
        this->timeseries = timeseries;
    }

    const Matrix ACF::fit() const
    {
        std::vector<double> acf(T);

        // https://stats.stackexchange.com/questions/493807/how-to-calculate-the-acf-and-pacf-for-time-series
        #pragma omp parallel for schedule(guided)
        for (const auto& j : std::views::iota(0UL, T)) {
            auto numerator = 0.0;
            for (const auto& t : std::views::iota(j, T)) {
                numerator += (timeseries[t] - u) * (timeseries[t - j] - u);
            }

            auto denominator = 0.0;
            for (const auto& t : std::views::iota(0UL, T)) {
                denominator += pow(timeseries[t] - u, 2);
            }

            acf[j] = numerator / denominator;
        }

        return Matrix({ acf });
    }
};
