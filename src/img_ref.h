#pragma once

#include <string>

namespace nicefig
{
    struct img_ref_t
    {
        std::string fname;
        std::array<double, 2> pos;
        double wid;
        img_ref_t(
            const std::array<double, 2>& pos_in,
            const double& wid_in,
            const std::string& fname_in) : fname{fname_in}, pos{pos_in}, wid{wid_in} {}
        
        std::string to_tikz() const
        {
            return "\\node[inner sep=0pt,anchor=south west] (russell) at (" + std::to_string(pos[0]) + "," + std::to_string(pos[1]) + ") {\\includegraphics[width=" + std::to_string(wid) + "pt]{" + fname + "}};";
        }
    };
}