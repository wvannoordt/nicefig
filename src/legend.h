#pragma once
#include "pen.h"
#include "axis_label.h"
#include "primitives.h"
#include "textnode.h"
#include "sym.h"

namespace nicefig
{
    struct legend_t
    {
        point_t                   x;
        anchor_node               anchor;
        pen_t                     border;
        bool                      has_border;
        std::vector<pen_t>        pens;
        std::vector<sym_t>        syms;
        std::vector<std::string>  names;
        std::array<int, 2>        dims;
        double                    linelength; //normalized by the width of the box
        double                    linespace; //normalized by the width of the box
        std::array<double, 2>     spacing;
        tikz_fontsize_t           font;
        bool                      center_anchor;
        double                    hscale = 1.0;
        
        std::string to_tikz() const
        {
            std::stringstream ss;
            int max_str_len = -1;
            for (const auto& n: names) max_str_len = hscale*std::max(max_str_len, int(n.length()));
            
            //central cell size
            std::array small_cell_size{max_str_len*font.ptsize(), font.ptsize()};
            std::array big_cell_size  {(1.0+spacing[0]+linelength+linespace)*small_cell_size[0], (1.0+spacing[1])*small_cell_size[1]};
            std::array bnd_size       {big_cell_size[0]*dims[0], big_cell_size[1]*dims[1]};
            // print(bnd_size[0], bnd_size[1]);
            // print(x[0], x[1]);
            // bnd_size[0] = 10;
            // bnd_size[1] = 10;
            bnd_t bound_box = from_location(x, bnd_size, anchor);
            
            const double dx = (bound_box[1]-bound_box[0])/dims[0];
            const double dy = (bound_box[3]-bound_box[2])/dims[1];
            int ct = 0;
            for (int j = 0; j < dims[1]; ++j)
            {
                for (int i = 0; i < dims[0]; ++i)
                {
                    if (ct < names.size())
                    {
                        point_t px{bound_box[0]+i*dx, bound_box[2]-j*dy};
                        bnd_t cell = from_location(px, big_cell_size, anchor_northwest);
                    
                        point_t left_anch{cell[0] + (linelength+linespace)*small_cell_size[0], 0.5*(cell[3] + cell[2])};
                        if (center_anchor) left_anch[0] = 0.5*(cell[0] + cell[1]);
                        point_t pstart = left_anch;
                        point_t pend   = left_anch;
                        pstart[0] -= (linelength+linespace)*small_cell_size[0];
                        pend[0]   -= (linespace)*small_cell_size[0];
                        // ss << sketch(cell, border) << "\n";
                        ss << sketch(pstart, pend, pens[ct]) << "\n";
                        
                        point_t pxt;
                        pxt[0] = 0.5*(pstart[0] + pend[0]);
                        pxt[1] = 0.5*(pstart[1] + pend[1]);
                        const auto stt = get_sym_tikz(syms[ct], pxt);
                        ss << stt << "\n";
                        
                        textnode_t node;
                        node.font = font;
                        node.text = names[ct];
                        node.anchor = anchor_west;
                        node.rotate = 0.0;
                        node.x = left_anch;
                        ss << node.to_tikz() << "\n";
                    }
                    ++ct;
                }
            }
            if (has_border) ss << sketch(bound_box, border);
            return ss.str();
        }
    };
}