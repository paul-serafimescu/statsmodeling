#include <linalg.hpp>

namespace matrix
{
    const MatrixDim Matrix::get_dim() const
    {
        return MatrixDim { .rows = _cols.size(), .cols = _cols[0].size() };
    }

    bool MatrixDim::operator==(const MatrixDim &rhs) const
    {
        return (rows == rhs.rows) && (cols == rhs.cols);
    }

    bool MatrixDim::operator!=(const MatrixDim &rhs) const
    {
        return (rows != rhs.rows) || (cols != rhs.cols);
    }

    Matrix::Matrix(std::vector<std::vector<double>> c) {
        _cols = c;
    }

    /**
     * @brief 
     * 
     * @param N number of rows
     * @param M number of columns
     * @param Z range of values [-Z, Z]
     * @return Matrix<int64_t> 
     */
    const Matrix from_random(size_t N, size_t M, double Z)
    {
        std::random_device seed;
        std::default_random_engine generator(seed());
        std::uniform_real_distribution<double> distribution(-Z, Z);

        std::vector<std::vector<double>> columns(N);
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < N; i++) {
            std::vector<double> c(M);
            for (size_t j = 0; j < M; j++) {
                c[j] = distribution(generator);;
            }
            columns[i] = c;
        }

        return Matrix(columns);
    }

    const Matrix from(std::initializer_list<std::initializer_list<double>> lst)
    {
        std::vector<std::vector<double>> columns(lst.size());
        size_t i = 0;
        for (const auto& l : lst) {
            columns[i++] = std::vector<double>(l);
        }

        return Matrix(columns);
    }

    const Matrix Matrix::augment_left(double value) const
    {
        auto dim = get_dim();
        auto c = matrix::from_zeroes(dim.rows, dim.cols + 1);

        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < dim.rows; i++) {
            for (size_t j = 0; j < dim.cols + 1; j++) {
                if (j == 0) {
                    c._cols[i][j] = value;
                } else {
                    c._cols[i][j] = _cols[i][j - 1];
                }
            }
        }

        return c;
    }

    Matrix from_zeroes(size_t N, size_t M) 
    {
        std::vector<std::vector<double>> columns(N);
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < N; i++) {
            std::vector<double> c(M, 0.0);
            columns[i] = c;
        }

        return Matrix(columns);
    }

    Matrix from_zeroes(MatrixDim dim) 
    {
        const size_t N = dim.rows, M = dim.cols;
        std::vector<std::vector<double>> columns(N);
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < N; i++) {
            std::vector<double> c(M, 0.0);
            columns[i] = c;
        }

        return Matrix(columns);
    }

    const Matrix from_identity(size_t N)
    {
        std::vector<std::vector<double>> columns(N);
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < N; i++) {
            std::vector<double> c(N, 0.0);
            c[i] = 1.0;
            columns[i] = c;
        }

        return Matrix(columns);
    }

    const std::vector<double> &Matrix::operator[](const int64_t index) const
    {
        return _cols[index < 0 ? _cols.size() + index : index];
    }

    const std::vector<double> &Matrix::operator[](const size_t index) const
    {
        return _cols[index];
    }

    bool Matrix::operator==(const Matrix &rhs) const
    {
        const double EPSILON = 1e-4; // seems reasonable enough

        auto dimensions = get_dim();
        if (dimensions != rhs.get_dim()) {
            return false;
        }

        auto r = dimensions.rows, c = dimensions.cols;
        bool res = true;

        #pragma omp parallel for reduction(&&:res)
        for (size_t i = 0; i < r; i++) {
            for (size_t j = 0; j < c; j++) {
                if ((std::fabs(_cols[i][j] - rhs._cols[i][j])) > EPSILON) {
                    res = false;
                }
            }
        }

        return res;
    }

    const std::string Matrix::to_string() const
    {
        auto dimensions = get_dim();
        std::stringstream ss;
        for (size_t i = 0; i < dimensions.rows; i++) {
            ss << "[";
            for (size_t j = 0; j < dimensions.cols; j++) {
                ss << _cols[i][j];
                if (j < dimensions.cols - 1) { // not last one
                    ss << " ";
                }
            }
            ss << "]\n";
        }

        return ss.str();
    }

    const Matrix Matrix::operator*(double rhs) const
    {
        auto c = matrix::from_zeroes(get_dim());

        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < _cols.size(); i++) {
            for (size_t j = 0; j < _cols[0].size(); j++) {
                c._cols[i][j] = rhs * _cols[i][j];
            }
        }

        return c;
    }

    const Matrix operator*(double lhs, const Matrix& mat)
    {
        return mat * lhs;
    }

    const Matrix Matrix::transpose() const
    {
        auto dimensions = get_dim();
        auto c = matrix::from_zeroes(dimensions.cols, dimensions.rows);

        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < dimensions.rows; i++) {
            for (size_t j = 0; j < dimensions.cols; j++) {
                c._cols[j][i] = _cols[i][j];
            }
        }

        return c;
    }

    const Matrix Matrix::inverse() const
    {
        auto dimensions = get_dim();
        if (dimensions.rows != dimensions.cols) {
            throw; // figure out later
        }

        const size_t N = dimensions.rows;

        auto c = matrix::from_identity(N); // augmented matrix

        auto a = _cols;

        // Gauss-Jordan Elimination
        for (size_t i = 0; i < N; i++) {
            if (a[i][i] == 0.0) throw; // inversion not possible
            // #pragma omp parallel for schedule(guided)
            for (size_t j = 0; j < N; j++) {
                if (i != j) {
                    double r = a[j][i] / a[i][i];
                    for (size_t k = 0; k < N; k++) {
                        a[j][k] -= r * a[i][k];
                        c._cols[j][k] -= r * c[i][k];
                    }
                }
            }
        }

        // make diagonal equal to one
        #pragma omp parallel for schedule(static)
        for(size_t i = 0; i < N; i++) {
            for(size_t j = 0; j < N; j++) {
                c._cols[i][j] = c[i][j] / a[i][i];
            }
		}

        return c;
    }

    const Matrix Matrix::operator+(const Matrix &rhs) const
    {
        auto dim = get_dim();
        if (dim != rhs.get_dim()) {
            throw; // figure this out later
        }

        auto c = matrix::from_zeroes(dim);

        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < dim.rows; i++) {
            for (size_t j = 0; j < dim.cols; j++) {
                c._cols[i][j] = _cols[i][j] + rhs[i][j];
            }
        }

        return c;
    }

    const Matrix Matrix::operator-(const Matrix &rhs) const
    {
        auto dim = get_dim();
        if (dim != rhs.get_dim()) {
            throw; // figure this out later
        }

        auto c = matrix::from_zeroes(dim);

        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < dim.rows; i++) {
            for (size_t j = 0; j < dim.cols; j++) {
                c._cols[i][j] = _cols[i][j] - rhs[i][j];
            }
        }

        return c;
    }

    const Matrix Matrix::operator*(const Matrix &rhs) const
    {
        auto lhs_dim = get_dim();
        auto rhs_dim = rhs.get_dim();

        assert(lhs_dim.cols == rhs_dim.rows); // make sure this is valid

        auto c = matrix::from_zeroes(lhs_dim.rows, rhs_dim.cols);
        #pragma omp parallel for schedule(static)
        for (size_t i = 0; i < lhs_dim.rows; i++) {
            for (size_t k = 0; k < lhs_dim.cols; k++) {
                for (size_t j = 0; j < rhs_dim.cols; j++) {
                    c._cols[i][j] += _cols[i][k] * rhs._cols[k][j];
                }
            }
        }

        return c;
    }
};
