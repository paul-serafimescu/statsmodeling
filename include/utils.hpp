#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>

#include <omp.h>

namespace statsmodeling
{
    #pragma GCC diagnostic ignored "-Wsubobject-linkage" // oopsie
    namespace
    {
        enum class ModelStatus
        {
            INITIALIZED,
            FITTED,
            ERROR
        };
    };

    class FitResult
    {
    public:
        FitResult();
    private:
    };

    // for doing operations on columns or rows or whatever
    const std::vector<double> map(double (*f)(double), std::vector<double> &v);
    const std::vector<double> log(std::vector<double> &v);
    const std::vector<double> sqrt(std::vector<double> &v);
    const std::vector<double> exp(std::vector<double> &v);
    const std::vector<double> pow(std::vector<double> &v, int64_t p);
};

#endif
