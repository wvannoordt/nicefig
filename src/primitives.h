#pragma once
#include <string>
#include "pen.h"
namespace nicefig
{
    constexpr static pen_t default_pen{2.0, solid_style, {0.0, 0.0, 0.0}};
    
    static std::string sketch(const bnd_t& box, const pen_t& pen, const rgb_t& fill)
    {
        std::stringstream ss;
        ss << "\\draw";
        ss << pen.to_tikz(fill);
        ss << " (" << box[0] << ", " << box[2] << ") --";
        ss << " (" << box[1] << ", " << box[2] << ") --";
        ss << " (" << box[1] << ", " << box[3] << ") --";
        ss << " (" << box[0] << ", " << box[3] << ") -- cycle;";
        return ss.str();
    }
    
    static std::string sketch(const bnd_t& box, const pen_t& pen = default_pen)
    {
        std::stringstream ss;
        ss << "\\draw";
        ss << pen.to_tikz();
        ss << " (" << box[0] << ", " << box[2] << ") --";
        ss << " (" << box[1] << ", " << box[2] << ") --";
        ss << " (" << box[1] << ", " << box[3] << ") --";
        ss << " (" << box[0] << ", " << box[3] << ") -- cycle;";
        return ss.str();
    }
    
    static std::string sketch(const point_t& x0, const point_t x1, const pen_t& pen = default_pen)
    {
        std::stringstream ss;
        ss << "\\draw";
        ss << pen.to_tikz();
        ss << " (" << x0[0] << ", " << x0[1] << ") --";
        ss << " (" << x1[0] << ", " << x1[1] << ");";
        return ss.str();
    }
    
    static std::string sketch(const std::vector<point_t>& data, const pen_t& pen = default_pen)
    {
        std::stringstream ss;
        ss << "\\draw";
        ss << pen.to_tikz();
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            if (i > 0) ss << " -- ";
            ss << "(" << data[i][0] << "," << data[i][1] << ")";
        }
        ss << ";";
        return ss.str();
    }
    
    static std::string sketch(const std::vector<point_t>& data, const pen_t& pen, const rgb_t& fill)
    {
        std::stringstream ss;
        ss << "\\draw";
        ss << pen.to_tikz(fill);
        for (std::size_t i = 0; i < data.size(); ++i)
        {
            if (i > 0) ss << " -- ";
            ss << "(" << data[i][0] << "," << data[i][1] << ")";
        }
        ss << ";";
        return ss.str();
    }
}