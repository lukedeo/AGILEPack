#ifndef WEIGHTING___HH
#define WEIGHTING___HH 

#include "ROOT.hh"
//----------------------------------------------------------------------------

namespace agile {
namespace root {

class weighting
{
public:
    weighting() : charm_pct(0.11), light_pct(0.54), bottom_pct(0.35) {}
    
    weighting &gen_hist(const agile::root::tree_reader &tree_buf, int n_entries = 1000, int start = 0)
    {   
        std::vector<std::string> flavors({"bottom", "charm", "light"});

        int m_num_pt_bins = tree_buf.get_binning()["categ_pt"].size();
        int m_num_eta_bins = tree_buf.get_binning()["categ_eta"].size();

        charm_correction.resize(m_num_pt_bins);
        bottom_correction.resize(m_num_pt_bins);
        light_correction.resize(m_num_pt_bins);
        charm_hist.resize(m_num_pt_bins);
        bottom_hist.resize(m_num_pt_bins);
        light_hist.resize(m_num_pt_bins);

        for (int i = 0; i < m_num_pt_bins; ++i)
        {
            charm_correction[i].resize(m_num_eta_bins);
            bottom_correction[i].resize(m_num_eta_bins);
            light_correction[i].resize(m_num_eta_bins);

            charm_hist[i].resize(m_num_eta_bins);
            bottom_hist[i].resize(m_num_eta_bins);
            light_hist[i].resize(m_num_eta_bins);
        }
        for (int cat_pT = 0; cat_pT < m_num_pt_bins; ++cat_pT)
        {
            for (int cat_eta = 0; cat_eta < m_num_eta_bins; ++cat_eta)
            {
                light_hist[cat_pT][cat_eta] = 0;
                charm_hist[cat_pT][cat_eta] = 0;
                bottom_hist[cat_pT][cat_eta] = 0;
            }
        }

        for (int i = start; i < n_entries; ++i)
        {
            tree_buf.get_entry(i);
            if (tree_buf.entry_in_range())
            {
                auto vars = tree_buf(i, flavors);
                if (vars["bottom"] > 0.5)
                {
                    bottom_hist[(int)tree_buf(i, "categ_pt")][(int)tree_buf(i, "categ_eta")] += 1;
                }
                else if (vars["light"] > 0.5)
                {
                    light_hist[(int)tree_buf(i, "categ_pt")][(int)tree_buf(i, "categ_eta")] += 1;
                }
                else
                {
                    charm_hist[(int)tree_buf(i, "categ_pt")][(int)tree_buf(i, "categ_eta")] += 1;
                }
            }
        }

        for (int cat_pT = 0; cat_pT < m_num_pt_bins; ++cat_pT)
        {
            for (int cat_eta = 0; cat_eta < m_num_eta_bins; ++cat_eta)
            {
                bottom_correction[cat_pT][cat_eta] = std::min(std::max(light_hist[cat_pT][cat_eta], 1.0) / ((1.0) * std::max(bottom_hist[cat_pT][cat_eta], 1.0)), 20.0);
                charm_correction[cat_pT][cat_eta] = std::min(std::max(light_hist[cat_pT][cat_eta], 1.0) / (5.0 * std::max(charm_hist[cat_pT][cat_eta], 1.0)), 20.0);
            }
        }
    }
//----------------------------------------------------------------------------
    weighting &charm_percentage(const double &pct)
    {
        charm_pct = pct; 
        return *this;
    }
//----------------------------------------------------------------------------
    weighting &light_percentage(const double &pct)
    {
        light_pct = pct; 
        return *this;
    }
//----------------------------------------------------------------------------
    weighting &bottom_percentage(const double &pct)
    {
        bottom_pct = pct; 
        return *this;
    }
//----------------------------------------------------------------------------

    ~weighting() = default;

private:
    double charm_pct, light_pct, bottom_pct;
    std::vector<std::vector<double>> charm_correction, 
                                    light_correction, 
                                    bottom_correction, 
                                    charm_hist, 
                                    bottom_hist, 
                                    light_hist;
};


}
}



#endif