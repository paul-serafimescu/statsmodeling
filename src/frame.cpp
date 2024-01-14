#include "frame.hpp"

using matrix::Matrix;

namespace frame
{
    Frame from_csv(const char *filename)
    {
        using statsmodeling::utils::is_number;

        // open csv file
        std::fstream f;
        f.open(filename, std::ios::in);

        if (!f.is_open()) {
            throw "could not open file";
        }

        // to store the body of the csv in matrix form
        std::vector<std::vector<double>> content;

        // intermediate vars
        std::string line, elem;

        // frame to be returned
        Frame frame;

        bool first_row = true;

        // store the column names
        std::vector<std::string> cols;

        while (std::getline(f, line)) {

            std::vector<double> row;

            std::stringstream ss(line);

            while (std::getline(ss, elem, ',')) {
                if (first_row && is_number<long double>(elem)) {
                    first_row = false;
                }

                if (first_row) {
                    cols.push_back(elem);
                } else {
                    row.push_back(std::strtold(elem.c_str(), nullptr));
                }
            }

            if (!first_row) {
                content.push_back(row);
            }
        }

        frame.columns = cols;
        frame.data = Matrix(content);
        frame.dataT = frame.data.T();

        f.close();

        return frame;
    }

    size_t Frame::column_count() const
    {
        return columns.size();
    }

    const std::string Frame::display() const
    {
        std::stringstream ss;

        // print out column names
        for (const auto& col : columns) {
            ss << col << " ";
        } ss << "\n" << std::string(ss.str().length(), '-') << "\n";

        ss << data.to_string(); // this should be better in the future

        return ss.str();
    }

    const Matrix Frame::operator[](std::string index) const
    {
        size_t idx = SIZE_MAX;
        for (size_t i = 0; i < columns.size(); i++) {
            if (columns[i] == index) {
                idx = i;
                break;
            }
        }

        if (idx == SIZE_MAX) {
            throw "indexing frame column"; // change later probably
        }

        return Matrix({ dataT[idx] }).T();
    }

    const Matrix Frame::operator[](std::initializer_list<std::string> indices) const
    {
        size_t idx = SIZE_MAX;
        std::vector<std::vector<double>> cols;

        for (const auto& index : indices) {
            for (size_t i = 0; i < columns.size(); i++) {
                if (columns[i] == index) {
                    idx = i;
                    break;
                }
            }

            if (idx == SIZE_MAX) {
                throw "indexing frame columns";
            }

            cols.push_back(dataT[idx]);
        }

        return Matrix(cols).T();
    }
};
