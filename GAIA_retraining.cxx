#include "Base"
#include "include/parser.hh"



template <class T, class U>
std::ostream& operator << ( std::ostream& o, const std::map<T, U> &m)
{
    for (auto &e : m)
    {
        o << e.first << ":\n" << e.second << std::endl;
    }
    return o;
}


void complain(const std::string &complaint);

//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{

    optionparser::parser p("A CLI for retraining GAIA.");
//----------------------------------------------------------------------------
    p.add_option("--file", "-f")    .help("Pass at least one file to add to a TChain for testing.")
                                    .mode(optionparser::store_mult_values);
    //----------------------------------------------------------------------------
    p.add_option("--tree", "-t")    .help("Name of the TTree to extract.")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--load")          .help("Name of a YAML neural network file to load.")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--config", "-c")  .help("Branch config file")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--learning")      .help("Pass a learning rate.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.1);
    //----------------------------------------------------------------------------
    p.add_option("--momentum")      .help("Pass a momentum value.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.5);
    //----------------------------------------------------------------------------
    p.add_option("--regularize")    .help("Pass an l2 norm regularizer.")
                                    .mode(optionparser::store_value)
                                    .default_value(0.00001);
    p.add_option("--batch")         .help("Mini-batch size.")
                                    .mode(optionparser::store_value)
                                    .default_value(10);
    //----------------------------------------------------------------------------
    p.add_option("--load")          .help("Name of a YAML neural network file to load to begin training")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--config", "-c")  .help("pass a config file")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--struct")        .help("pass the network structure")
                                    .mode(optionparser::store_mult_values);
    //----------------------------------------------------------------------------
    p.add_option("--deepauto", "-d").help("how many autoencoders to train?")
                                    .mode(optionparser::store_value)
                                    .default_value(-1);
    //----------------------------------------------------------------------------
    p.add_option("--type", "-T")    .help("Can be one of 'regress', 'multiclass', or 'binary'.")
                                    .mode(optionparser::store_value)
                                    .default_value("regress");
    //----------------------------------------------------------------------------
    p.add_option("--verbose", "-v") .help("Make the output verbose");
    //----------------------------------------------------------------------------
    p.add_option("--weights", "-w") .help("print a file with the first layer weight matrix.")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("-start")          .help("Start index for training. (Default = 0)")
                                    .mode(optionparser::store_value)
                                    .default_value(0);
    //----------------------------------------------------------------------------
    p.add_option("-end")            .help("End index for training. (Default, whole tree)")
                                    .mode(optionparser::store_value)
                                    .default_value(-1);  
    //----------------------------------------------------------------------------
    p.add_option("-uepochs")        .help("Number of passes over the Trees for Unsupervised Pretraining(Default = 5)")
                                    .mode(optionparser::store_value)
                                    .default_value(5);
    //----------------------------------------------------------------------------
    p.add_option("-sepochs")        .help("Number of passes over the Trees for Supervised Training(Default = 10)")
                                    .mode(optionparser::store_value)
                                    .default_value(10);
    //----------------------------------------------------------------------------
    p.add_option("--formula", "-F") .help("Specify Model Formula")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("-start")          .help("Start index for testing/training. (Default = 0)")
                                    .mode(optionparser::store_value)
                                    .default_value(0);
    //----------------------------------------------------------------------------
    p.add_option("-end")            .help("End index for testing/training. (Default, whole tree)")
                                    .mode(optionparser::store_value)
                                    .default_value(-1);
    //----------------------------------------------------------------------------
    p.add_option("--predict")       .help("Prediction mode!");
//----------------------------------------------------------------------------

    p.eat_arguments(argc, argv);

    if (!p.get_value("file")) complain("need to pass at least one file.");
    if (!p.get_value("tree")) complain("need to pass a tree name.");
    if (!p.get_value("config")) complain("need a config file for variable specification.");
    if (!p.get_value("formula")) complain("need a model formula to train.");
    if (!p.get_value("struct")) complain("need to pass a network structure.");
    
    std::vector<std::string> root_files(p.get_value<std::vector<std::string>>("file"));

    std::string ttree_name =    p.get_value<std::string>("tree"),
                config_file =   p.get_value<std::string>("config"),
                save_file =     p.get_value<std::string>("save"),
                model_formula = p.get_value<std::string>("formula");


    double  learning =    p.get_value<double>("learning"), 
            momentum =    p.get_value<double>("momentum"),
            regularizer = p.get_value<double>("regularize");


    int     deepauto =    p.get_value<int>("deepauto"),
            start =       p.get_value<int>("start"),
            end =         p.get_value<int>("end"),
            uepochs =      p.get_value<int>("uepochs"),
            sepochs =      p.get_value<int>("sepochs"),
            batch =       p.get_value<int>("batch");

    bool    verbose =     p.get_value("verbose");

    std::vector<int> structure = p.get_value<std::vector<int>>("struct");

    if (deepauto < 0)
    {
        deepauto = structure.size();
    }

//----------------------------------------------------------------------------

    agile::root::tree_reader tree_buf;

    for (auto &file : root_files) tree_buf.add_file(file, ttree_name);

    tree_buf.set_branches(config_file);

    agile::model_frame frame;

    frame.add_dataset(std::move(
        tree_buf.get_dataframe(start, end - start, verbose)));

    frame.model_formula(model_formula);
    frame.generate(verbose);
    frame.scale(verbose);

//----------------------------------------------------------------------------
    
    agile::neural_net net;
    net.load_model_frame_config(frame);
    net.check(false);

// Reweighting
//----------------------------------------------------------------------------
    int n_estimate = frame.X().rows() / 10;
    std::vector<std::vector<double>> charm_correction, 
                                     light_correction, 
                                     bottom_correction, 
                                     charm_hist, 
                                     bottom_hist, 
                                     light_hist;

    int m_num_pt_bins = tree_buf.get_binning()["categ_pt"].size();
    int m_num_eta_bins = tree_buf.get_binning()["categ_eta"].size();

    charm_correction.resize(m_num_pt_bins);
    bottom_correction.resize(m_num_pt_bins);
    light_correction.resize(m_num_pt_bins);
    charm_hist.resize(m_num_pt_bins);
    bottom_hist.resize(m_num_pt_bins);
    light_hist.resize(m_num_pt_bins);
    double charm_pct = 0.10, bottom_pct = 0.35, light_pct = 0.55;
    // for (int i = 0; i < m_num_pt_bins; ++i)
    // {
    //     charm_correction[i].resize(m_num_eta_bins);
    //     bottom_correction[i].resize(m_num_eta_bins);
    //     light_correction[i].resize(m_num_eta_bins);

    //     charm_hist[i].resize(m_num_eta_bins);
    //     bottom_hist[i].resize(m_num_eta_bins);
    //     light_hist[i].resize(m_num_eta_bins);
    // }
    // for (int cat_pT = 0; cat_pT < m_num_pt_bins; ++cat_pT)
    // {
    //     for (int cat_eta = 0; cat_eta < m_num_eta_bins; ++cat_eta)
    //     {
    //         light_hist[cat_pT][cat_eta] = 0;
    //         charm_hist[cat_pT][cat_eta] = 0;
    //         bottom_hist[cat_pT][cat_eta] = 0;
    //     }
    // }

    // for (int i = 0; i < n_estimate; ++i)
    // {
    //     at(i);
    //     if ((fabs(get_value("eta")) < 2.5) && (get_value("pt") > 20) && (get_value("flavor_truth_label") < 8) && (get_value("pt") < 1000))
    //     {
    //         if (cast_as_int(*variables["light"]) == 1)
    //         {
    //             light_hist[cast_as_int(*(variables["cat_pT"]))][cast_as_int(*(variables["cat_eta"]))] += 1;
    //         }
    //         else if (cast_as_int(*variables["charm"]) == 1)
    //         {
    //             charm_hist[cast_as_int(*(variables["cat_pT"]))][cast_as_int(*(variables["cat_eta"]))] += 1;
    //         }
    //         else if (cast_as_int(*variables["bottom"]) == 1)
    //         {
    //             bottom_hist[cast_as_int(*(variables["cat_pT"]))][cast_as_int(*(variables["cat_eta"]))] += 1;
    //         }
    //     }

    // }












    // net.from_yaml(load_file);

    // std::vector<std::string> input_vars {"pt", "bottom", "charm", "light", "MV1"};


    // std::cout << "prob_bottom, bottom, charm, light, pt, MV1" << std::endl;
    // for (int i = start; i < end; ++i)
    // {
    //     auto pred = net.predict_map(TR(i, net.get_inputs()));

    //     auto control = TR(i, input_vars);

    //     std::cout << pred["bottom"] << "," << control["bottom"] << "," << control["charm"] << "," << control["light"] << "," << control["pt"] << "," << control["MV1"] << std::endl;
    // }



    return 0;
}


void complain(const std::string &complaint)
{
    std::cerr << "Error: " << complaint << std::endl;
    exit(1);
}
