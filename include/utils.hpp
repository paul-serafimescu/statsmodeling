#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>
#include <optional>
#include <map>
#include <omp.h>

#include <linalg.hpp>

using matrix::Matrix;

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
        FitResult(Matrix params, bool has_const);
        const std::string display() const;
        double get_param(std::string param) const;
    private:
        std::optional<double> constant;
        std::map<std::string, double> params;
    };


    namespace utils
    {
        // for doing operations on columns or rows or whatever
        const std::vector<double> map(double (*f)(double), std::vector<double> &v);
        const std::vector<double> log(std::vector<double> &v);
        const std::vector<double> sqrt(std::vector<double> &v);
        const std::vector<double> exp(std::vector<double> &v);
        const std::vector<double> pow(std::vector<double> &v, int64_t p);

        template<typename T>
        bool is_number(const std::string& s)
        {
            T n;
            return((std::istringstream(s) >> n >> std::ws).eof());
        }
    };
};

#endif
