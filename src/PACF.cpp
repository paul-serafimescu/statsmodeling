/**
 * @file PACF.cpp
 * @author Paul Serafimescu (pserafimescu9@ucla.edu)
 * @brief Partial AutoCorrelation Function Implementation
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "PACF.hpp"

#include <iostream>

using namespace statsmodeling;

namespace statsmodeling
{
    PACF::PACF(std::initializer_list<double> t)
    {
        std::vector<double> ts(t);
        T = t.size();
        u = utils::mean(ts);
        timeseries = ts;
    }

    PACF::PACF(std::vector<double> ts)
    {
        T = timeseries.size();
        u = utils::mean(ts);
        timeseries = ts;
    }

    PACF::PACF(const Matrix& timeseriesM)
    {
        auto dim = timeseriesM.get_dim();

        if (dim.cols != 1) {
            std::stringstream ss;
            ss << "detected " << dim.cols << " series, should have only 1";
            throw ss.str().c_str();
        }

        auto ts = timeseriesM.T()[0UL];

        T = ts.size();
        u = utils::mean(ts);
        timeseries = ts;
    }

    const std::vector<double> PACF::difference(const std::vector<double> x) const
    {
        std::vector<double> res(x);
        for (const auto& i : std::views::iota(0UL, x.size())) {
            res[i] -= u;
        }

        return res;
    }

    const Matrix PACF::fit() const
    {
        std::vector<double> pacf(T - 2);
        // https://stats.stackexchange.com/questions/493807/how-to-calculate-the-acf-and-pacf-for-time-series

        auto ts = difference(timeseries);

        #pragma omp parallel for schedule(guided)
        for (const auto& j : std::views::iota(1UL, T - 1)) {
            auto t0 = utils::slice(ts, 0, ts.size() - j);
            auto y = Matrix({ t0 }).T();

            std::vector<std::vector<double>> X_;
            for (const auto& k : std::views::iota(0UL, j)) {
                std::vector<double> Xk = utils::shift(ts, k + 1);
                X_.push_back(utils::slice(Xk, 0, ts.size() - j));
            }

            auto X = Matrix(X_).T();

            try {
                auto reg = OLS(false);
                auto result = reg.fit(X, y);

                std::stringstream ss;
                ss << "B" << (j);
                pacf[j - 1] = result.get_param(ss.str());
            } catch (const char *err) { 
                std::cerr << err << std::endl;
                pacf[j - 1] = 0;
            }
        }

        return Matrix({ pacf }).T();
    }
};
