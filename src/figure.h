#pragma once

#include <string>
#include <fstream>
#include "def_types.h"
namespace nicefig
{
    struct figure_t
    {
        rgb_t backgr;
        std::ofstream hdl;
        figure_t(const std::string& fname, const rgb_t& backgr_in = {1.0, 1.0, 1.0}) : hdl{fname}, backgr{backgr_in}
        {
            std::string bckgr = "{rgb,255:red," +  std::to_string((int)(255*backgr[0]))
                + ";green,"                 + std::to_string((int)(255*backgr[1]))
                + ";blue,"                  + std::to_string((int)(255*backgr[2])) + "}";
            hdl << "\\usetikzlibrary{backgrounds}\n\\tikzset{background rectangle/.style={fill=" << bckgr << "}}\n\\tikzset{every picture/.style={line width=0.75pt}}\n";
            hdl << "\\begin{tikzpicture}[x=0.75pt,y=0.75pt,yscale=1,xscale=1]\n";
        }
        
        ~figure_t() { hdl << "\\end{tikzpicture}\n"; }
        
        template <typename rhs_t>
        figure_t& operator << (const rhs_t& rhs)
        {
            hdl << rhs.to_tikz() << "\n";
            return *this;
        }
    };
}