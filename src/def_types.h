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
}