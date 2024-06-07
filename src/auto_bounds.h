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
}