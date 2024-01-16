#include "utils.hpp"

namespace plt = matplotlibcpp;

namespace statsmodeling
{
    FitResult::FitResult(Matrix params, Matrix X, Matrix y, bool has_const)
    {
        exog = X;
        endog = y;
        _params = params;
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
        const std::vector<double> map(std::function<double(double)> f, const std::vector<double> &v)
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
            return map([](double x) { return std::log(x); }, v);
        }

        const std::vector<double> sqrt(std::vector<double> &v)
        {
            return map([](double x) { return std::sqrt(x); }, v);
        }

        const std::vector<double> exp(std::vector<double> &v)
        {
            return map([](double x) { return std::exp(x); }, v);
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

        const std::vector<double> shift(std::vector<double> v, size_t n)
        {
            std::vector<double> result(v.size() - n);
            std::copy(v.begin() + n, v.end(), result.begin());
            return result;
        }

        const std::vector<double> slice(std::vector<double> v, size_t l, size_t r)
        {
            if (r < l) {
                throw "invalid bounds";
            }

            std::vector<double> result(r - l);
            std::copy(v.begin() + l, v.begin() + r, result.begin());
            return result;
        }

        double mean(const std::vector<double> &v)
        {
            return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
        }

        double max(const std::vector<double>& v)
        {
            double maximum = -INFINITY;
            #pragma omp parallel for reduction(std::max:maximum)
            for (const auto& e : v) {
                maximum = std::max(e, maximum);
            }

            return maximum;
        }

        double min(const std::vector<double>& v)
        {
            double minimum = INFINITY;
            #pragma omp parallel for reduction(std::min:minimum)
            for (const auto& e : v) {
                minimum = std::min(e, minimum);
            }

            return minimum;
        }

        double dot(const std::vector<double>& v, const std::vector<double>& w)
        {
            if (v.size() != w.size()) {
                throw "size should be the same in dot product";
            }

            auto res = 0.0;
            #pragma omp parallel for reduction(+:res)
            for (const auto& i : std::views::iota(0UL, v.size())) {
                res += v[i] * w[i];
            }

            return res;
        }

        const std::vector<double> mult(const std::vector<double>& v, double w)
        {
            return utils::map(([&](double x) { return x * w; }), v);
        }

        const std::vector<double> add(const std::vector<double>& v, double w)
        {
            return utils::map(([&](double x) { return x + w; }), v);
        }

        // inclusive [start, end] with num_intervals elements
        const std::vector<double> linspace(int64_t start, int64_t end, size_t num_intervals)
        {
            if (end < start) {
                throw "invalid bounds";
            }

            double dstart = static_cast<double>(start);
            double dend = static_cast<double>(end);
            double dnum_intervals = static_cast<double>(num_intervals);

            std::vector<double> v(num_intervals);

            double delta = (dend - dstart) / (dnum_intervals - 1);
            v[0] = dstart;

            #pragma omp parallel for schedule(static)
            for (size_t i = 1; i < num_intervals; i++) {
                v[i] = dstart + i * delta;
            }

            return v;
        }
    };

    // this is still a work in progress, i think
    // more testing and effort should go into it in the future
    // but for now this current functionality works for simple OLS
    void FitResult::plot() const
    {
        auto y_dim = endog.get_dim();
        auto X_dim = exog.get_dim();
        auto params_dim = _params.get_dim();

        auto Xt = exog.T();
        auto Yt = endog.T();

        // Prepare data.
        auto step = 1;
        size_t n = (size_t)std::round(1.5 * utils::max(Xt[0UL]));
        std::vector<double> x = utils::linspace(0, n, 100);
        std::vector<double> z(x.size());

        // extract constant if exists
        auto p = _params[0UL];
        auto constant = 0.0;
        if (p.size() > X_dim.cols) {
            constant = p[0];
            p = utils::shift(p, 1);
        }

        // generate fitted values
        for(int i = 0; i < x.size(); i++) {
            auto m = utils::mult(p, x[i]);
            z.at(i) = std::accumulate(m.begin(), m.end(), constant);
        }

        // plot the fitted model
        plt::plot(x, z);

        // plot the scattered values
        std::map<std::string, std::string> kwargs;
        kwargs["marker"] = "o";
        kwargs["linestyle"] = "";
        plt::plot(Xt[0UL], Yt[0UL], kwargs);

        plt::title("statsmodeling plot");
        plt::legend();
        plt::show();
    }
};
