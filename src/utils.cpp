#include "utils.hpp"
namespace statsmodeling
{
    FitResult::FitResult(Matrix params, bool has_const)
    {
        const size_t first_row_assert = 0;
        if (has_const)
            constant = params[first_row_assert][0];
        else constant = std::nullopt;

        if (params.get_dim().rows > 1) throw "creating fitresult";
        for (size_t i = has_const; i < params[first_row_assert].size(); i++) {
            std::stringstream ss;
            ss << "B" << (i + (1 - has_const));
            this->params.insert(std::pair<std::string, double>(ss.str(), params[first_row_assert][i]));
        }
    }

    double FitResult::get_param(std::string param) const
    {
        return param == "const" ? constant.value_or(0) : params.at(param);
    }

    const std::string FitResult::display() const
    {
        std::stringstream ss;
        // print out the constant
        ss << "B0: ";
        if (constant.has_value())
            ss << constant.value();
        else ss << "NULL";
        ss << "\n";

        // other params
        for (const auto& item : params) {
            ss << item.first << ": " << item.second << "\n";
        }

        return ss.str();
    }

    namespace utils
    {
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
};
