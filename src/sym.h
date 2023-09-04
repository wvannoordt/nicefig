#pragma once

#include "def_types.h"

namespace nicefig
{
    enum mark_shape
    {
        no_shape,
        square_shape
    };
    
    struct sym_t
    {
        constexpr static int circle_div = 24;
        rgb_t color;
        mark_shape shape;
        pen_t border;
        double size;
    };
    
    static sym_t no_sym{{0.0,0.0,0.0}, no_shape, default_pen, 0.0};
    
    static std::string get_sym_tikz(const sym_t& sym, const point_t& pt)
    {
        switch (sym.shape)
        {
            case no_shape:
            {
                return "";
            }
            case square_shape:
            {
                bnd_t b;
                b[0] = pt[0]-0.5*sym.size;
                b[1] = pt[0]+0.5*sym.size;
                b[2] = pt[1]-0.5*sym.size;
                b[3] = pt[1]+0.5*sym.size;
                return sketch(b, sym.border, sym.color);
            }
        }
        return get_sym_tikz(no_sym, pt);
    }
}