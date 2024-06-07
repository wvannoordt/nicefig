#pragma once

#include "figure.h"

namespace nicefig
{
    struct scoped_clip_t
    {
        figure_t* f;
        scoped_clip_t(figure_t& f_in, bnd_t bnd)
        {
            f = &f_in;
            *f << "\\begin{scope}\n\\clip(" + std::to_string(bnd[0]) + "," + std::to_string(bnd[2]) + ") rectangle (" + std::to_string(bnd[1]) + "," + std::to_string(bnd[3]) + ");";
        }
        
        ~scoped_clip_t()
        {
            *f << "\\end{scope}";
        }
    };
}