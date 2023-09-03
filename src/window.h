#pragma once
#include <cmath>
#include <array>
#include <concepts>
#include "primitives.h"
#include "curve.h"
#include "axis_label.h"
#include "legend.h"
#include "sym.h"

namespace nicefig
{
    struct window_t
    {
        using handle_t = std::size_t;
        
        bnd_t                               bnd, axisbound;
        std::array<axis_label_t, 4>         axis_labels;
        bool xlog = false, ylog = false;
        
        std::vector<curve_t<double>> curves;
        std::vector<pen_t>   pens;
        std::vector<sym_t>   syms;
        double sym_scale_fac = 2.0;
        
        pen_t boxpen{2.25, solid_style, {0,0,0}};
        
        double min (int i) const {return bnd[2*i];}
        double max (int i) const {return bnd[2*i+1];}
        double size(int i) const {return max(i) - min(i);}
        double mid (int i) const {return 0.5*(min(i) + max(i));}
        
        double val_min (int i) const {return axisbound[2*i];}
        double val_max (int i) const {return axisbound[2*i+1];}
        double val_size(int i) const {return val_max(i) - val_min(i);}
        double val_mid (int i) const {return 0.5*(val_min(i) + val_max(i));}
        
        axis_label_t&       axis_label(int i)       {return axis_labels[i];}
        const axis_label_t& axis_label(int i) const {return axis_labels[i];}
        
        window_t(const bnd_t& bin, const bnd_t& axb) : bnd{bin}, axisbound{axb}
        {
            for (int i = 0; i < 4; ++i)
            {
                int dir = i/2;
                axis_labels[i].normal_dir  = dir;
                axis_labels[i].tangent_dir =(dir+1)%2;
                axis_labels[i].min_ax_val  = val_min(axis_labels[i].tangent_dir);
                axis_labels[i].max_ax_val  = val_max(axis_labels[i].tangent_dir);
            }
        }
        
        template <typename data_t>
        requires (std::same_as<data_t, double>)
        handle_t add(const curve_t<data_t>& n, const pen_t pen = default_pen, const sym_t sym = no_sym)
        {
            curves.push_back(n);
            pens.push_back(pen);
            syms.push_back(sym);
            return curves.size() - 1;
        }
        
        //Gets the coordinates on the canvas given the plot coordinates
        point_t map(const point_t& xin) const
        {
            point_t output
            {
                min(0) + size(0)*(xin[0] - axisbound[0])/(axisbound[1] - axisbound[0]),
                min(1) + size(1)*(xin[1] - axisbound[2])/(axisbound[3] - axisbound[2])
            };
            
            if (xlog)
            {
                output[0] = min(0) + size(0)*(std::log10(xin[0]) - std::log10(axisbound[0]))/(std::log10(axisbound[1]) - std::log10(axisbound[0]));
                if (xin[0] < 1e-10) output[0] = axisbound[1];
            }
            
            if (ylog)
            {
                //todo
            }
            
            return output;
        }
        
        void set_xlog(bool v)
        {
            xlog = v;
        }
        void set_ylog(bool v)
        {
            ylog = v;
        }
        
        legend_t create_legend(const std::vector<std::string>& names, const anchor_node& anch, const point_t location) const
        {
            legend_t output;
            output.names         = names;
            output.pens          = pens;
            output.syms          = syms;
            for (auto& s: output.syms) s.size *= sym_scale_fac;
            output.anchor        = anch;
            output.x             = location;
            output.border        = boxpen;
            output.has_border    = false;
            output.dims          = {int(names.size()), 1};
            output.linelength    = 0.15;
            output.linespace     = 0.02;
            output.spacing       = {0.05, 0.1};
            output.font          = tikz_fontsize_t{8};
            output.center_anchor = true;
            return output;
        }
        
        std::string to_tikz() const
        {
            
            std::stringstream output;
            output << sketch(bnd, boxpen) << "\n";
            output << "\\begin{scope}\n";
            output << "\\clip (" << min(0) << "," << min(1) << ") rectangle (" << max(0) << "," << max(1) << ");\n";
            for (std::size_t j = 0; j < curves.size(); ++j)
            {
                const auto& l   = curves[j];
                const auto& p   = pens[j];
                const auto& sym = syms[j];
                std::vector<point_t> locals;
                locals.reserve(l.size());
                for (std::size_t i = 0; i < l.size(); ++i)
                {
                    locals.push_back(map({l.x(i), l.y(i)}));
                }
                output << sketch(locals, p) << "\n";
                for (std::size_t i = 0; i < l.size(); ++i)
                {
                    auto px = map({l.x(i), l.y(i)});
                    const auto stt = get_sym_tikz(sym, px);
                    output << stt << "\n";
                }
            }
            output << "\\end{scope}\n";
            
            int ct = 0;
            for (const auto& label:axis_labels)
            {
                if (label.enabled)
                {
                    int pm  = ct%2;
                    
                    point_t center{mid(0), mid(1)};
                    point_t anch;
                    int nor_dir = label.normal_dir;
                    int tan_dir = label.tangent_dir;
                    anch[tan_dir]   = center[tan_dir];
                    anch[nor_dir]   = min(nor_dir) - label.offset*size(nor_dir);
                    if (pm == 1) anch[nor_dir]    = max(nor_dir) + label.offset*size(nor_dir);
                    double rotate = 0.0;
                    if (nor_dir == 0) rotate = -270.0;
                    std::string chr = "north";
                    if (nor_dir == 0 && pm != 1) chr = "south";
                    //todo: improve this trash
                    output << "\\draw (" << anch[0] << "," << anch[1] << ") node [anchor=" << chr << "][inner sep=0.75pt] [font=" << label.fontsize.to_tikz();
                    output << ",rotate=" << rotate << "]";
                    output << "[align=left] {";
                    output << label.text;
                    output << "};\n";
                    
                    const auto draw_ticks = [&](const std::vector<double>& ticks, double len, const pen_t& tpen, bool flip, bool write_text, tikz_fontsize_t fnt, bool loglabel)
                    {
                        for (const auto& t: ticks)
                        {
                            const double tol = 1e-5;
                            if (t <= val_max(tan_dir) + tol && t >= val_min(tan_dir) - tol)
                            {
                                point_t mp;
                                mp[nor_dir] = val_min(nor_dir);
                                if (pm==1) mp[nor_dir] = val_max(nor_dir);
                                mp[tan_dir] = t;
                                point_t mapped = map(mp);
                                point_t end   = mapped;
                                point_t ptext = mapped;
                                int mult = 1;
                                if ((pm==0) == !flip) mult = -1;
                                end[nor_dir] = mapped[nor_dir]   +     mult*len*size(nor_dir);
                                ptext[nor_dir] = mapped[nor_dir] + 1.5*mult*len*size(nor_dir);
                                output << sketch(mapped, end, tpen);
                                std::string                  nchr = "north";
                                if (tan_dir == 0 && pm == 1) nchr = "south";
                                if (tan_dir == 1 && pm == 0) nchr = "east";
                                if (tan_dir == 1 && pm == 1) nchr = "west";
                                if (write_text)
                                {
                                    output << "\\draw (" << ptext[0] << "," << ptext[1] << ") node [anchor=" << nchr << "][inner sep=0.75pt] [font=" << fnt.to_tikz();
                                    output << ",rotate=0]";
                                    output << "[align=left] {";
                                    std::stringstream pp;
                                    if (loglabel)
                                    {
                                        double lval = std::log10(t);
                                        pp << "$10^{" << lval << "}$";
                                    }
                                    else
                                    {
                                        pp << t;
                                    }
                                    output << pp.str();
                                    output << "};\n";
                                }
                            }
                        }
                    };
                    
                    draw_ticks(label.min_ticks, label.min_tick_len, label.min_pen, label.min_ticks_flipped, false, label.axisfontsize, label.loglabel);
                    draw_ticks(label.maj_ticks, label.maj_tick_len, label.maj_pen, label.maj_ticks_flipped, label.enable_tick_text,  label.axisfontsize, label.loglabel);
                }
                ++ct;
            }
            
            return output.str();
        }
    };
}