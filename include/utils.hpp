#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <vector>
#include <tuple>
#include <optional>
#include <map>
#include <limits>
#include <ranges>
#include <omp.h>
#include <functional>
#include <matplotlib-cpp/matplotlib.hpp>

#include "linalg.hpp"

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
        FitResult(Matrix params, Matrix X, Matrix y, bool has_const);
        const std::string display() const;
        double get_param(std::string param) const;
        void plot() const;
    private:
        std::optional<double> constant;
        std::map<std::string, double> params;
        Matrix exog;
        Matrix endog;
        Matrix _params;
    };

    namespace utils
    {
        // for doing operations on columns or rows or whatever
        const std::vector<double> map(std::function<double(double)> f, const std::vector<double> &v);
        const std::vector<double> log(std::vector<double> &v);
        const std::vector<double> sqrt(std::vector<double> &v);
        const std::vector<double> exp(std::vector<double> &v);
        const std::vector<double> pow(std::vector<double> &v, int64_t p);

        // for manipulating data shape
        const std::vector<double> shift(std::vector<double> v, size_t n);
        const std::vector<double> slice(std::vector<double> v, size_t l, size_t r);

        double mean(const std::vector<double> &v);
        double max(const std::vector<double>& v);
        double min(const std::vector<double>& v);
        double dot(const std::vector<double>& v, const std::vector<double>& w);
        const std::vector<double> mult(const std::vector<double>& v, double w);
        const std::vector<double> add(const std::vector<double>& v, double w);
        // maybe add reduce?

        const std::vector<double> linspace(int64_t start, int64_t end, size_t num_intervals);

        template<typename T>
        bool is_number(const std::string& s)
        {
            T n;
            return((std::istringstream(s) >> n >> std::ws).eof());
        }
    };
};

#endif
