#pragma once

namespace nicefig
{
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
            while (val > min_ax_val) val = gen(--i);
            while (val < max_ax_val)
            {
                val = gen(i++);
                ticks.push_back(val);
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