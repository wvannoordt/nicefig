#pragma once

#include <string>
#include <fstream>

namespace nicefig
{
    struct figure_t
    {
        std::ofstream hdl;
        figure_t(const std::string& fname) : hdl{fname}
        {
            hdl << "\\tikzset{every picture/.style={line width=0.75pt}}\n";
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