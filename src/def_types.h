#pragma once
#include <array>
namespace nicefig
{
    using real_t  = double;
    using bnd_t   = std::array<double, 4>;
    using point_t = std::array<double, 2>;
    using rgb_t   = std::array<float, 3>;
    
    static constexpr rgb_t invert(const rgb_t& c)
    {
        return rgb_t{float(1.0)-c[0], float(1.0)-c[1], float(1.0)-c[2]};
    }
    
    static bnd_t scale_x(const bnd_t& ibnd, const double& scale)
    {
        double r = 0.5*(ibnd[1] - ibnd[0]);
        double c = 0.5*(ibnd[1] + ibnd[0]);
        auto output = ibnd;
        output[0] = c - scale*r;
        output[1] = c + scale*r;
        return output;
    }
    
    static bnd_t scale_y(const bnd_t& ibnd, const double& scale)
    {
        double r = 0.5*(ibnd[3] - ibnd[2]);
        double c = 0.5*(ibnd[3] + ibnd[2]);
        auto output = ibnd;
        output[2] = c - scale*r;
        output[3] = c + scale*r;
        return output;
    }
}