/**
 * @file ACF.hpp
 * @author Paul Serafimescu (pserafimescu9@ucla.edu)
 * @brief AutoCorrelation Function Generation
 * @version 0.1
 * @date 2024-01-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef ACF_HPP
#define ACF_HPP

#include <vector>
#include <sstream>
#include <ranges>
#include <iostream>
#include <omp.h>

#include "linalg.hpp"
#include "utils.hpp"

using matrix::Matrix;

namespace statsmodeling
{
    class ACF
    {
    public:
        ACF(std::initializer_list<double> timeseries);
        ACF(std::vector<double> timeseries);
        ACF(const Matrix& timeseries);
        const Matrix fit() const;
    private:
        double u;
        size_t T;
        std::vector<double> timeseries;
    };
};

#endif
