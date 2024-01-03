#include "OLS.hpp"

using matrix::Matrix;

namespace statsmodeling
{
    OLS::OLS(bool add_const)
    {
        use_const = add_const;
        status = ModelStatus::INITIALIZED;
    }

    const FitResult OLS::fit(Matrix& X, Matrix& y) const
    {
        auto Xs = use_const ? X.augment_left(1.0) : X; 
        auto Xt = Xs.transpose();
        auto result =  (Xt * Xs).inverse() * Xt * y;
        return FitResult(); // TODO
    }
};
