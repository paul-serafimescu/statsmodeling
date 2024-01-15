/**
 * @file PACF.hpp
 * @author Paul Serafimescu (pserafimescu@ucla.edu)
 * @brief Partial AutoCorrelation Function
 * @version 0.1
 * @date 2024-01-14
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PACF_HPP
#define PACF_HPP

#include <ranges>
#include <bits/stdc++.h>
#include <sstream>

#include "linalg.hpp"
#include "utils.hpp"
#include "OLS.hpp"

using matrix::Matrix;

namespace statsmodeling
{
    class PACF
    {
    public:
        PACF(std::initializer_list<double> ts);
        PACF(std::vector<double> ts);
        PACF(const Matrix& ts);
        const Matrix fit() const;
    private:
        size_t T;
        std::vector<double> timeseries;
        double u;

        // helper functions
        const std::vector<double> difference(const std::vector<double>) const;
    };
};

#endif
