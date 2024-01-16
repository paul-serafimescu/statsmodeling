#include "OLS.hpp"

#include <iostream>
using matrix::Matrix;

namespace statsmodeling
{
    OLS::OLS(bool add_const)
    {
        use_const = add_const;
        status = ModelStatus::INITIALIZED;
    }

    const FitResult OLS::fit(Matrix& X, Matrix& y)
    {
        auto Xs = use_const ? X.augment_left(1.0) : X; 
        auto Xt = Xs.T();
        auto result =  (Xt * Xs).inv() * Xt * y;
        status = ModelStatus::FITTED;
        return FitResult(result.T(), X, y, use_const);
    }
};
