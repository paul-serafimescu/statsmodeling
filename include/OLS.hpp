#ifndef OLS_HPP
#define OLS_HPP

#include "linalg.hpp"
#include "utils.hpp"

namespace statsmodeling
{
    using matrix::Matrix;

    class OLS
    {
    public:
        OLS(bool add_const = true);
        const FitResult fit(Matrix& X, Matrix& y);
    private:
        ModelStatus status;
        bool use_const;
    };
};

#endif
