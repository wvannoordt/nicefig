#pragma once

#include <array>

namespace nicefig
{
    enum line_style
    {
        dash_style,
        dot_style,
        densedot_style,
        solid_style
    };
    
    namespace detail
    {
        std::string tstr(const line_style& st)
        {
            switch(st)
            {
                case dash_style:     return "dashed";
                case dot_style:      return "dotted";
                case densedot_style: return "densely dotted";
                case solid_style:    return "solid";
                default:             return "solid";
            }
        }
    }
    
    using rgb_t = std::array<float, 3>;
    
    struct pen_t
    {
        double width = 2.0;
        line_style style = solid_style;
        rgb_t color = {0.0,0.0,0.0};
        
        std::string to_tikz() const
        {
            return std::string("[") + detail::tstr(style)
                + ", line width = " + std::to_string(width)
                + ", color = {rgb,255:red," + std::to_string((int)(255*color[0]))
                + ";green,"                 + std::to_string((int)(255*color[1]))
                + ";blue,"                  + std::to_string((int)(255*color[2])) + "}]";
        }
    };
}