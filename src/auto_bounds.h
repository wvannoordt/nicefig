#pragma once

#include "curve.h"

namespace nicefig
{
    template <typename val_t>
    static bnd_t auto_bounds(const curve_t<val_t>& data)
    {
        bnd_t output{0,1,0,1};
        if (data.size() < 1) return output;
        double xmin = data.x(0);
        double xmax = data.x(0);
        double ymin = data.y(0);
        double ymax = data.y(0);
        
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            xmin = std::min(xmin, data.x(i));
            xmax = std::max(xmax, data.x(i));
            ymin = std::min(ymin, data.y(i));
            ymax = std::max(ymax, data.y(i));
        }
        double yinflate = 0.08;
        double ya  = 0.5*(ymin + ymax);
        double del = 0.5*(ymax - ymin);
        output = {xmin, xmax, ya-(1+yinflate)*del, ya+(1+yinflate)*del};
        
        return output;
    }
    
    template <typename... curves_t>
    static bnd_t auto_bounds(const curves_t&... data)
    {
        std::array bnds{auto_bounds(data)...};
        
        bnd_t output = bnds[0];
        for (int i = 0; i < bnds.size(); ++i)
        {
            output[0] = std::min(output[0], bnds[i][0]);
            output[1] = std::max(output[1], bnds[i][1]);
            output[2] = std::min(output[2], bnds[i][2]);
            output[3] = std::max(output[3], bnds[i][3]);
        };
        
        return output;
    }
}