#pragma once

#include <string>
#include <sstream>
#include "axis_label.h"
namespace nicefig
{
    enum anchor_node
    {
        anchor_northwest,
        anchor_north,
        anchor_northeast,
        anchor_west,
        anchor_middle,
        anchor_east,
        anchor_southwest,
        anchor_south,
        anchor_southeast
    };
    
    static std::string anch_str(const anchor_node& node)
    {
        switch (node)
        {
            case anchor_northwest: return "north west";
            case anchor_north:     return "north";
            case anchor_northeast: return "north east";
            case anchor_west:      return "west";
            case anchor_middle:    return "center";
            case anchor_east:      return "east";
            case anchor_southwest: return "south west";
            case anchor_south:     return "south";
            case anchor_southeast: return "south east";
            default: return "center";
        }
    }
    
    static bnd_t from_location(const point_t& x, const std::array<double, 2>& size, const anchor_node& anchor = anchor_middle)
    {
        std::array<double, 2> displ = [&]()
        {
            switch (anchor)
            {
                case anchor_northwest: return std::array{ 0.0,  0.0};
                case anchor_north:     return std::array{-0.5,  0.0};
                case anchor_northeast: return std::array{-1.0,  0.0};
                case anchor_west:      return std::array{ 0.0,  0.5};
                case anchor_middle:    return std::array{-0.5,  0.5};
                case anchor_east:      return std::array{-1.0,  0.5};
                case anchor_southwest: return std::array{ 0.0,  1.0};
                case anchor_south:     return std::array{-0.5,  1.0};
                case anchor_southeast: return std::array{-1.0,  1.0};
                default:               return std::array{ 0.0,  0.0};
            }
        }();
        
        bnd_t output;
        output[0] = x[0] + displ[0]*size[0];
        output[1] = output[0] + size[0];
        output[2] = x[1] + displ[0]*size[1];
        output[3] = output[2] + size[1];
        
        return output;
    }
    
    struct textnode_t
    {
        tikz_fontsize_t font;
        std::string text = "";
        anchor_node anchor;
        double rotate;
        point_t x;
        
        std::string to_tikz() const
        {
            std::stringstream ss;
            ss << "\\draw (" << x[0] << "," << x[1] << ") node [anchor=" << anch_str(anchor) << "][inner sep=0.75pt] [font=" << font.to_tikz();
            ss << ",rotate=" << rotate << "]";
            ss << "[align=left] {";
            ss << text;
            ss << "};";
            return ss.str();
        }
    };
}