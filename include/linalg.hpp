#include <vector>
#include <random>
#include <sstream>

#include <omp.h>

namespace matrix
{
    struct MatrixDim
    {
        const size_t rows;
        const size_t cols;

        bool operator==(const MatrixDim &rhs) const;
        bool operator!=(const MatrixDim &rhs) const;
    };

    class Matrix
    {
    private:
        std::vector<std::vector<double>> _cols;
    public:
        Matrix(std::vector<std::vector<double>> c);
        const Matrix operator*(const Matrix &rhs);
        const Matrix operator+(const Matrix &rhs) const;
        const Matrix operator-(const Matrix &rhs) const;
        const Matrix operator*(double rhs) const;
        const Matrix transpose() const;
        const Matrix inverse() const;
        bool operator==(const Matrix &rhs) const;
        const std::vector<double> &operator[](int64_t index) const;
        const std::vector<double> &operator[](size_t index) const;
        const MatrixDim get_dim() const;
        const std::string to_string() const;

        friend const Matrix operator*(double lhs, const Matrix& mat);
    };

    const Matrix from_random(size_t, size_t, double);
    Matrix from_zeroes(size_t, size_t);
    Matrix from_zeroes(MatrixDim dim);
    const Matrix from_identity(size_t);
};

// TODO:
namespace statsmodeling
{
    class OLS
    {
    public:
    private:
    };
};
