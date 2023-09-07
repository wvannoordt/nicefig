#pragma once

#include <vector>
#include "def_types.h"

namespace nicefig
{
    struct colormap_t
    {
        std::vector<rgb_t> data;
        void add(const rgb_t& d) { data.push_back(d); }
        rgb_t sample(double ii) const
        {
            double i = 1.0 - ii;
            if (data.size() == 0) return {0.0,0.0,0.0};
            if (data.size() == 1) return data[0];
            if (i < 1e-6)         return data[0];
            if (i > 0.9999)       return data.back();
            
            double dx    = 1.0/(data.size()-1);
            int j        = int(i/dx);
            rgb_t c0     = data[j];
            rgb_t c1     = data[j+1];
            double a     = (i-j*dx)/dx;
            double b     = 1.0 - a;
            rgb_t output{b*c0[0] + a*c1[0], b*c0[1] + a*c1[1], b*c0[2] + a*c1[2]};
            
            return output;
        }
    };
}