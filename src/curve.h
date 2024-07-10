#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
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
        data_t& x(std::size_t i) { return xdat[i];}
        data_t& y(std::size_t i) { return ydat[i];}
        
        curve_t& operator += (const curve_t& rhs)
        {
            for (std::size_t i = 0; i < rhs.size(); ++i)
            {
                xdat.push_back(rhs.xdat[i]);
                ydat.push_back(rhs.ydat[i]);
            }
            return *this;
        }
        
        void xsort()
        {
            std::vector<std::size_t> idx;
            for (const auto& i: xdat) idx.push_back(idx.size());
            std::sort(idx.begin(), idx.end(), [&](const auto& a, const auto& b) { return xdat[a] < xdat[b]; });
            auto xold = xdat;
            auto yold = ydat;
            for (std::size_t i = 0; i < idx.size(); ++i)
            {
                xdat[i] = xold[idx[i]];
                ydat[i] = yold[idx[i]];
            }
        }
    };
    
    template <typename data_t = real_t>
    static curve_t<data_t> read(const std::string& filename)
    {
        curve_t<data_t> output;
        std::ifstream mf(filename);
        if(mf.fail())
        {
            throw std::runtime_error("Requested file \"" + filename + "\" could not be opened.");
        }
        std::string line;
        while(std::getline(mf, line))
        {
            for (auto& i:line)
            {
                if (i==',') i = ' ';
            }
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