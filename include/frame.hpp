#ifndef FRAME_HPP
#define FRAME_HPP

#include <string>
#include <fstream>
#include <ios>
#include <vector>
#include <sstream>
#include <iostream>

#include "linalg.hpp"
#include "utils.hpp"

namespace frame
{
    class Frame
    {
    private:
        std::vector<std::string> columns = {};
        Matrix data = {{}};
        Matrix dataT = {{}};
    public:
        friend Frame from_csv(const char *filename);
        size_t column_count() const;
        const std::string display() const;
        const Matrix operator[](std::string index) const;
        const Matrix operator[](std::initializer_list<std::string> indices) const;
        void add_column(const std::string& new_col_name, const Matrix& new_col);
    };

    extern Frame from_csv(const char *filename);
};

#endif
