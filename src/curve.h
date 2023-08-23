#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "def_types.h"
namespace nicefig
{
    template <typename data_t = real_t>
    struct curve_t
    {
        std::vector<data_t> xdat, ydat;
        std::size_t size() const {return xdat.size();}
        const data_t& x(std::size_t i) const { return xdat[i];}
        const data_t& y(std::size_t i) const { return ydat[i];}
    };
    
    template <typename data_t = real_t>
    static curve_t<data_t> read(const std::string& filename)
    {
        curve_t<data_t> output;
        std::ifstream mf(filename);
        std::string line;
        while(std::getline(mf, line))
        {
            std::istringstream iss(line);
            data_t x, y;
            iss >> x;
            iss >> y;
            output.xdat.push_back(x);
            output.ydat.push_back(y);
        }
        return output;
    }
}