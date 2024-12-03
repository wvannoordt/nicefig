#pragma once

#include "def_types.h"

namespace nicefig
{
    
    
    namespace detail
    {
        template <typename func_t>
        static std::vector<point_t> circ(const point_t& ctr, double rad, int ndiv, double offst, const func_t& r_of_theta)
        {
            std::vector<point_t> output;
            double pi = 3.1415926535;
            double dtheta = 2*pi / ndiv;
            for (int i = 0; i <= ndiv; ++i)
            {
                double theta = (offst*pi/180.0) + dtheta*i;
                double radloc = rad*r_of_theta(theta);
                output.push_back({ctr[0] + radloc*cos(theta), ctr[1] + radloc*sin(theta)});
            }
            return output;
        }
        
        static std::vector<point_t> circ(const point_t& ctr, double rad, int ndiv, double offst)
        {
            return circ(ctr, rad, ndiv, offst, [](double){return 1.0;});
        }
    }
    
    enum mark_shape
    {
        no_shape,
        square_shape,
        triangle_shape,
        diamond_shape,
        inv_triangle_shape,
        circle_shape,
        star3_shape,
        star4_shape,
        star5_shape,
        star6_shape,
        star7_shape,
        star8_shape
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
            case triangle_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 3, 90);
                return sketch(points, sym.border, sym.color);
            }
            case diamond_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 4, 90);
                return sketch(points, sym.border, sym.color);
            }
            case inv_triangle_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 3, -90);
                return sketch(points, sym.border, sym.color);
            }
            case circle_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, sym.circle_div, 0);
                return sketch(points, sym.border, sym.color);
            }
            case star3_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 6, 0, [](double theta){ return 0.75 + 0.5*cos(3*theta); });
                return sketch(points, sym.border, sym.color);
            }
            case star4_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 8, 0, [](double theta){ return 0.75 + 0.25*cos(4*theta); });
                return sketch(points, sym.border, sym.color);
            }
            case star5_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 10, 0, [](double theta){ return 0.75 + 0.25*cos(5*theta); });
                return sketch(points, sym.border, sym.color);
            }
            case star6_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 12, 0, [](double theta){ return 0.75 + 0.25*cos(6*theta); });
                return sketch(points, sym.border, sym.color);
            }
            case star7_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 14, 0, [](double theta){ return 0.75 + 0.25*cos(7*theta); });
                return sketch(points, sym.border, sym.color);
            }
            case star8_shape:
            {
                auto points = detail::circ(pt, sym.size*0.7, 16, 0, [](double theta){ return 0.75 + 0.25*cos(8*theta); });
                return sketch(points, sym.border, sym.color);
            }
        }
        return get_sym_tikz(no_sym, pt);
    }
}