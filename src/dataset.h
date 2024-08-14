#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <map>
#include "def_types.h"

namespace nicefig
{
    struct dataset_t
    {
        std::map<std::string, int> names_to_idx;
        std::vector<std::vector<double>> curves;
        
        std::size_t size() const { return curves.size(); }
        
        template <typename str_t>
        std::vector<double>& operator[](const str_t& iname)
        {
            std::string name(iname);
            if (names_to_idx.find(name) == names_to_idx.end())
            {
                throw std::runtime_error("Could not find name \"" + name + "\" in dataset");
            }
            return (*this)[names_to_idx.at(name)];
        }
        
        template <typename str_t>
        const std::vector<double>& operator[](const str_t& iname) const
        {
            std::string name(iname);
            if (names_to_idx.find(name) == names_to_idx.end())
            {
                throw std::runtime_error("Could not find name \"" + name + "\" in dataset");
            }
            return (*this)[names_to_idx.at(name)];
        }
        
        std::vector<double>& operator[](const int idx)
        {
            if ((idx < 0) || (idx >= curves.size())) throw std::runtime_error("Index \"" + std::to_string(idx) + "\" out of range");
            return curves[idx];
        }
        
        const std::vector<double>& operator[](const int idx) const
        {
            if ((idx < 0) || (idx >= curves.size())) throw std::runtime_error("Index \"" + std::to_string(idx) + "\" out of range");
            return curves[idx];
        }
        
        template <typename i0_t, typename i1_t>
        curve_t<double> make_curve(const i0_t& i0, const i1_t& i1) const
        {
            return curve_t<double>{(*this)[i0], (*this)[i1]};
        }
    };
    
    static dataset_t read_data(const std::string& fname, int nskip = 0)
    {
        dataset_t output;
        std::ifstream mf(fname);
        constexpr static std::string_view dlms = ";, ";
        if (mf.fail()) throw std::runtime_error("Could not open file \"" + fname + "\" for dataset read");
        int num_cols = 0;
        
        std::string line, tmph;
        for (int i = 0; i < nskip; ++i) std::getline(mf, line);
        if (!std::getline(mf, tmph) || !(std::getline(mf, line))) throw std::runtime_error("Could not open file \"" + fname + "\": empty file");
        for (auto& i:line)
        {
            if (dlms.find(i) != std::string::npos) i = ' ';
        }
        for (auto& i:tmph)
        {
            if (dlms.find(i) != std::string::npos) i = ' ';
        }
        std::istringstream isstmph(tmph);
        double pp;
        isstmph >> pp;
        bool has_header = isstmph.fail();
        bool done = false;
        std::istringstream issline(line);
        while (true)
        {
            double tmp;
            issline >> tmp;
            if (issline.fail()) break;
            num_cols++;
        }
        
        
        
        mf.clear();
        mf.seekg(0);
        for (int i = 0; i < nskip; ++i) std::getline(mf, line);
        if (has_header)
        {
            std::string header;
            std::getline(mf, header);
            
            for (auto& i:header)
            {
                if (dlms.find(i) != std::string::npos) i = ' ';
            }
            
            std::istringstream hiss(header);
            for (int i = 0; i < num_cols; ++i)
            {
                std::string nm;
                hiss >> nm;
                if (hiss.fail()) throw std::runtime_error("Error reading file \"" + fname + "\": header does not contain enough (" + std::to_string(num_cols) + ") entries");
                output.names_to_idx[nm] = i;
            }
        }
        else
        {
            for (int i = 0; i < num_cols; ++i)
            {
                output.names_to_idx["data"+std::to_string(i)] = i;
            }
        }
        
        output.curves.resize(output.names_to_idx.size());
        //Finished reading header.
        
        //Now read the data.
        int line_num = 2;
        while(std::getline(mf, line))
        {
            for (auto& i:line)
            {
                if (dlms.find(i) != std::string::npos) i = ' ';
            }
            std::istringstream siss(line);
            for (int i = 0; i < num_cols; ++i)
            {
                double tmp;
                siss >> tmp;
                if (siss.fail()) throw std::runtime_error("Error reading file \"" + fname + "\": data does not contain enough (" + std::to_string(num_cols) + ") entries (line " + std::to_string(line_num) + ")");
                output[i].push_back(tmp);
            }
            line_num++;
        }
        
        return output;
    }
    
}