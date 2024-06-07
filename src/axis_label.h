#pragma once
#include <iostream>
#include <print.h>
namespace nicefig
{
    static struct tauto_ticks_t {} automatic_ticks;
    
    namespace detail
    {
        struct tick_coeff_result_t
        {
            double min_a, min_b, maj_a, maj_b;
        };
        
        static auto tick_coeff(double x0, double x1, int n)
        {
            double dx = (x1-x0)/n;
            std::array moduli{1.0, 0.5, 0.25, 0.2, 0.1, 0.05};
            int logval = log(dx)/log(10.0) + 1;
            double dx_tick_hat = dx / std::pow(10.0,logval);
            double modulus = moduli[0];
            const auto abs = [](const auto& a) { return (a > 0)?a:-a; };
            for (int i = 0; i < moduli.size(); ++i)
            {
                if (abs(moduli[i] - dx_tick_hat) < modulus) modulus = moduli[i];
            }
            double coeff = std::pow(10.0,logval)*modulus;
            
            int nwin = (x1-x0)/coeff;
            if (nwin < 4)
            {
                coeff /= 2.0;
            }
            
            if (nwin > 7)
            {
                coeff *= 2.0;
            }
            
            double coeff2 = coeff/5;
            int z0 = x0/coeff2;
            int z1 = x0/coeff;
            return tick_coeff_result_t{z0*coeff2, coeff2, z1*coeff, coeff};
        }
    }
    
    struct tikz_fontsize_t
    {
        constexpr static std::array vals
        {
            "\\tiny",
            "\\scriptsize",
            "\\footnotesize",
            "\\small",
            "\\normalsize",
            "\\large",
            "\\Large",
            "\\LARGE",
            "\\huge",
            "\\Huge"
        };
        
        constexpr static std::array sizes
        {
            7.5,   //"\\tiny",
            10.5,  //"\\scriptsize",
            12.0,  //"\\footnotesize",
            13.5,  //"\\small",
            15.0,  //"\\normalsize",
            18.0,  //"\\large",
            21.6,  //"\\Large",
            25.92, //"\\LARGE",
            31.11, //"\\huge",
            37.32  //"\\Huge"
        };
        
        int value;
        
        void operator ++ ()
        {
            value ++;
            if (value >= vals.size()) value = vals.size() - 1;
        }
        
        void operator -- ()
        {
            value --;
            if (value < 0) value = 0;
        }
        
        std::string to_tikz() const
        {
            return vals[value];
        }
        
        double ptsize() const 
        {
            return sizes[value];
        }
    };
    
    struct axis_label_t
    {
        bool loglabel = false;
        bool enabled = false;
        std::string text = "variable";
        double maj_tick_len = 0.03;//normalized by the frame length in the corresponding direction
        double min_tick_len = 0.015;
        double offset       = 0.1;
        int logbase         = 10;
        pen_t maj_pen{1.75, solid_style, {0.0, 0.0, 0.0}};
        pen_t min_pen{1.05, solid_style, {0.0, 0.0, 0.0}};
        tikz_fontsize_t fontsize{8}, axisfontsize{6};
        std::vector<double> min_ticks, maj_ticks;
        double min_ax_val, max_ax_val;
        int normal_dir, tangent_dir;
        bool min_ticks_flipped = false;
        bool maj_ticks_flipped = false;
        bool enable_tick_text  = true;
        
        void set_color(const rgb_t& col)
        {
            maj_pen.color = col;
            min_pen.color = col;
        }
        
        
        
        template <typename gen_t>
        void set_ticks(std::vector<double>& ticks, const gen_t gen)
        {
            int i = 0;
            double val = gen(0);
            while (val > min_ax_val)
            {
                val = gen(--i);
            }
            while (val < max_ax_val)
            {
                val = gen(i++);
                if ((val <= max_ax_val) && (val >= min_ax_val)) ticks.push_back(val);
            }
        }
        
        void set_minor_ticks(tauto_ticks_t)
        {            
            if (loglabel)
            {
                const auto lam = [&](int i)
                {                    
                    int b = 10;
                    int bp = b-1;
                    int imodbp = (bp + (i % bp)) % bp;
                    int ipp = (i-imodbp)/bp;
                    
                    int j = b*ipp + (imodbp+1);
                    
                    int imod = (b + (j % b)) % b;
                    int offst = j-imod;
                    int z = offst/b;
                    double coeff = std::pow(double(b), z);
                    return coeff*(imod);
                };
                
                this->set_minor_ticks(lam);
            }
            else
            {
                auto result = detail::tick_coeff(min_ax_val, max_ax_val, 7);
                this->set_minor_ticks([&](const int i){ return result.min_a + result.min_b*i; });
            }
        }
        
        void set_major_ticks(tauto_ticks_t)
        {
            if (loglabel)
            {
                this->set_major_ticks([&](int i){return std::pow(logbase, 1.0*i);});
            }
            else
            {
                auto result = detail::tick_coeff(min_ax_val, max_ax_val, 7);
                this->set_major_ticks([&](const int i){ return result.maj_a + result.maj_b*i; });
            }
        }
        
        template <typename gen_t>
        void set_minor_ticks(const gen_t& gen)
        {
            set_ticks(min_ticks, gen);
        }
        
        template <typename gen_t>
        void set_major_ticks(const gen_t& gen)
        {
            set_ticks(maj_ticks, gen);
        }
    };
}