#include "utils.hpp"

namespace statsmodeling
{
    FitResult::FitResult()
    {

    }

    const std::vector<double> map(double (*f)(double), std::vector<double> &v)
    {
        auto c = v;
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < c.size(); i++) {
            c[i] = f(c[i]);
        }

        return c;
    }

    const std::vector<double> log(std::vector<double> &v)
    {
        return map(std::log, v);
    }

    const std::vector<double> sqrt(std::vector<double> &v)
    {
        return map(std::sqrt, v);
    }

    const std::vector<double> exp(std::vector<double> &v)
    {
        return map(std::exp, v);
    }

    const std::vector<double> pow(std::vector<double> &v, int64_t p)
    {
        auto c = v;
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < c.size(); i++) {
            c[i] = std::pow(c[i], p);
        }

        return c;
    }
};
