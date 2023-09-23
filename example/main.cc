#include "nicefig.h"

int main(int argc, char** argv)
{
    nicefig::curve_t d0 = nicefig::read("data/Udns.dat");
    nicefig::curve_t d1 = nicefig::read("data/Utl.dat");
    nicefig::curve_t d2 = nicefig::read("data/Uym.dat");
    
    nicefig::curve_t g0 = nicefig::read("data/Tdns.dat");
    nicefig::curve_t g1 = nicefig::read("data/Ttl.dat");
    nicefig::curve_t g2 = nicefig::read("data/Tym.dat");    
    
    double max    = 800;
    double aspect = 0.3;
    
    nicefig::bnd_t ub{0.0,     0.5*max,  0.0,   aspect*max};
    nicefig::bnd_t ux{1e-3,    1e0,     0.0,   1.5};
    
    double delta = 0.08*max;
    nicefig::bnd_t tb{0.5*max + delta, max + delta,      0.0,   aspect*max};
    nicefig::bnd_t tx{1e-3,    1e0,      0.0,   10.0};
    
    nicefig::pen_t p0; p0.style = nicefig::solid_style;
    nicefig::pen_t p1; p1.style = nicefig::dash_style;
    nicefig::pen_t p2; p2.style = nicefig::densedot_style;
    
    nicefig::window_t u(ub, ux);
    u.add(d0, p0);
    u.add(d1, p1);
    u.add(d2, p2);
    
    u.set_xlog(true);
    
    u.axis_label(0).enabled = true;
    u.axis_label(0).text    = "$\\overline{u}/U_b$";
    u.axis_label(0).offset  = 0.15;
    u.axis_label(0).set_minor_ticks([](int i){return 0.04*i;});
    u.axis_label(0).set_major_ticks([](int i){return 0.4*i;});
    
    u.axis_label(2).enabled  = true;
    u.axis_label(2).loglabel = true;
    u.axis_label(2).text    = "$y/\\delta$";
    u.axis_label(2).set_minor_ticks([](int i)
    {
        if (0.001*i < 0.01)       return 0.001*i;
        if (0.01*(i-10)  < 0.1)   return 0.01*(i-10);
        if (0.1*(i-20)   < 1.0)   return 0.1*(i-20);
        if (1.0*(i-30)   < 10.0)  return 1.0*(i-30);
        return 0.01*i;
    });
    u.axis_label(2).set_major_ticks([](int i){return std::pow(10.0, 1.0*i);});
    
    
    nicefig::tikz_fontsize_t font{7};
    double offset = 0.03*max;
    nicefig::point_t x_leg{0.5*(ub[0] + tb[1]), ub[3]+offset};
    auto leg = u.create_legend({"DNS", "$T_0$", "$T_1$"}, nicefig::anchor_south, x_leg);
    
    leg.linelength = 0.4;
    
    nicefig::window_t t(tb, tx);
    t.add(g0, p0);
    t.add(g1, p1);
    t.add(g2, p2);
    
    t.set_xlog(true);
    
    t.axis_label(1).enabled = true;
    t.axis_label(1).text    = "$\\overline{T}/T_w$";
    t.axis_label(1).set_minor_ticks([](int i){return 0.25*i;});
    t.axis_label(1).set_major_ticks([](int i){return 2.0*i;});
    
    t.axis_label(2).enabled  = true;
    t.axis_label(2).loglabel = true;
    t.axis_label(2).text     = "$y/\\delta$";
    t.axis_label(2).set_minor_ticks([](int i)
    {
        if (0.001*i < 0.01)       return 0.001*i;
        if (0.01*(i-10)  < 0.1)   return 0.01*(i-10);
        if (0.1*(i-20)   < 1.0)   return 0.1*(i-20);
        if (1.0*(i-30)   < 10.0)  return 1.0*(i-30);
        return 0.01*i;
    });
    t.axis_label(2).set_major_ticks([](int i){return std::pow(10.0, 1.0*i);});
    
    nicefig::pen_t bpen{1.75, nicefig::solid_style, {0,0,0}};
    
    u.boxpen = bpen;
    t.boxpen = bpen;
    
    nicefig::figure_t fig("out.tex");
    
    // u.axis_label(0).min_ticks_flipped = true;
    // u.axis_label(0).maj_ticks_flipped = true;
    // u.axis_label(2).min_ticks_flipped = true;
    // u.axis_label(2).maj_ticks_flipped = true;
    
    // t.axis_label(1).min_ticks_flipped = true;
    // t.axis_label(1).maj_ticks_flipped = true;
    // t.axis_label(2).min_ticks_flipped = true;
    // t.axis_label(2).maj_ticks_flipped = true;
    
    fig << u;
    fig << t;
    fig << leg;
    
    return 0;
}
