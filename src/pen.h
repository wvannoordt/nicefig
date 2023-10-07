#pragma once

#include <array>

namespace nicefig
{
    enum line_style
    {
        dash_style,
        dot_style,
        densedot_style,
        solid_style,
        dashdot_style
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
                case dashdot_style:  return "dashdotted";
                case solid_style:    return "solid";
                default:             return "solid";
            }
        }
    }
    
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
        
        std::string to_tikz(const rgb_t& fill) const
        {
            return std::string("[") + detail::tstr(style) + ",line width = " + std::to_string(width)
                + ",color = {rgb,255:red," +  std::to_string((int)(255*color[0]))
                + ";green,"                 + std::to_string((int)(255*color[1]))
                + ";blue,"                  + std::to_string((int)(255*color[2])) + "}, draw opacity = 1,"
                + "fill = {rgb,255:red,"  + std::to_string((int)(255*fill[0]))
                + ";green,"                 + std::to_string((int)(255*fill[1]))
                + ";blue,"                  + std::to_string((int)(255*fill[2])) + "}, fill opacity=1]";
        }
    };
}