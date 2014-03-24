#include "weighting.hh"
#include "Base"
#include "include/parser.hh"
#include <time.h>

void config_info();

void complain(const std::string &complaint);

const std::string timestamp(void);


//----------------------------------------------------------------------------
int main(int argc, char const *argv[])
{
    std::string s("A simple CLI for AGILEPack centered around providing ");
    s += "functionality for \ntraining and testing Deep Learning";

    optionparser::parser p(s + " models on ROOT TTrees");

//----------------------------------------------------------------------------
    p.add_option("--file", "-f")    .help("Pass at least one file to add to a TChain for training.")
                                    .mode(optionparser::store_mult_values);
    //----------------------------------------------------------------------------
    p.add_option("--tree", "-t")    .help("Name of the TTree to extract.")
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("--save", "-s")    .help("Name of file to save the YAML neural network file to.")
                                    .mode(optionparser::store_value)
                                    .default_value(std::string("neural_net" + timestamp() + ".yaml"));
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
    std::string config_help = "Pass a configuration file for training specifications instead\n";
    config_help.append(25, ' ');
    config_help += "of over the command line. For help on formatting, pass\n";
    config_help.append(25, ' ');
    config_help += "the '-confighelp' flag. This overrides all paramaters from CLI.\n";
    config_help.append(25, ' ');
    config_help += "This is required for specifying training variables.\n";

    p.add_option("--config", "-c")  .help(config_help)
                                    .mode(optionparser::store_value);
    //----------------------------------------------------------------------------
    p.add_option("-confighelp")     .help("Display info about YAML training config files.");
    //----------------------------------------------------------------------------
    std::string struct_help = "Pass the structure of the neural network. For example, one\n";
    struct_help.append(25, ' ');
    struct_help += "could specify --struct=13 24 23 15 3 [other flags].";

    p.add_option("--struct")        .help(struct_help)
                                    .mode(optionparser::store_mult_values);
    //----------------------------------------------------------------------------
    std::string autoencoder_help = "Pretraining using deep autoencoders, if you pass an\n";
    autoencoder_help.append(25, ' ');
    autoencoder_help += "integer n, it will train the first n layers. It defaults to all.";

    p.add_option("--deepauto", "-d").help(autoencoder_help)
                                    .mode(optionparser::store_value)
                                    .default_value(-1);
    //----------------------------------------------------------------------------
    std::string type_help = "Specify the type of predicive target we are trying to \n";
    type_help.append(25, ' ');
    type_help += "work with. Can be one of 'regress', 'multiclass', or 'binary'.";

    p.add_option("--type", "-T")    .help(type_help)
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
    std::string prog_string = "Output progress files every (n) epochs. If n isn't passed,\n";
    prog_string.append(25, ' ');
    p.add_option("-prog")           .help(prog_string + "uses default. (Default = 1)")
                                    .mode(optionparser::store_value)
                                    .default_value(1);
    //----------------------------------------------------------------------------
    p.add_option("--formula", "-F") .help("Specify Model Formula")
                                    .mode(optionparser::store_value);
//----------------------------------------------------------------------------
    p.eat_arguments(argc, argv);

    if (p.get_value("confighelp")) config_info();

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
            batch =       p.get_value<int>("batch"),
            prog =        p.get_value<int>("prog");

    bool    verbose =     p.get_value("verbose");

    std::vector<int> structure = p.get_value<std::vector<int>>("struct");

    if (deepauto < 0)
    {
        deepauto = structure.size();
    }

//----------------------------------------------------------------------------

    agile::root::tree_reader TR;

    for (auto &file : root_files)
    {
       TR.add_file(file, ttree_name);
    }

    TR.set_branches(config_file);

    agile::root::weighting jet_weights;
    jet_weights.light_percentage(0.54)
               .charm_percentage(0.11)
               .bottom_percentage(0.35)
               .gen_hist(TR, 200000, 0, verbose);

//----------------------------------------------------------------------------
    agile::dataframe D = TR.get_dataframe(jet_weights, end - start, start, verbose);
    // agile::dataframe D = TR.get_dataframe(end - start, start, verbose);

    std::ofstream dframe("testfram.csv");
   
    dframe << D;

    dframe.close();

    agile::neural_net net;

    // net.load_config(config_file);
    
    net.add_data(D);

    layer_type net_type;
    std::string passed_target = p.get_value<std::string>("type");

//----------------------------------------------------------------------------

    if (passed_target == "regress") net_type = linear;
    else if (passed_target == "multiclass") net_type = softmax;
    else if (passed_target == "binary") net_type = sigmoid;
    else complain(
        "type of target needs to be one of 'regress', 'multiclass', or 'binary'.");
    
//----------------------------------------------------------------------------

    int i;
    for (i = 0; i < (structure.size() - 2); ++i)
    {
        if (i < deepauto)
        {
            net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
        }
        else
        {
            net.emplace_back(new layer(structure[i], structure[i + 1], sigmoid));
        }
        
    }
    if (i < deepauto)
    {
        net.emplace_back(new autoencoder(structure[i], structure[i + 1], net_type));
    }
    else
    {
        net.emplace_back(new layer(structure[i], structure[i + 1], net_type));
    }
    
    if (verbose)
    {
        std::cout << "\nParsing model formula " << model_formula << "...";
    }
    net.model_formula(model_formula, true, verbose);
    if (verbose)
    {
        std::cout << "Done." << std::endl;
    }

    net.set_learning(learning);
    net.set_regularizer(regularizer);
    net.set_momentum(momentum);
    net.set_batch_size(batch);
    
    net.check(0);



    if (verbose)
    {
        std::cout << "Performing Unsupervised Pretraining...";
    }
    net.train_unsupervised(uepochs, verbose);
    if (verbose)
    {
        std::cout << "\nPerforming Supervised Training...\n";
    }
    net.train_supervised(sepochs, verbose);
    if (verbose)
    {
        std::cout << "\nDone.\nSaving to " << save_file << "...";
    }
    net.to_yaml(save_file, TR.get_var_types(), TR.get_binning());
    if (verbose)
    {
        std::cout << "Done." << std::endl;
    }

    if (p.get_value("weights"))
    {
        std::ofstream wght(p.get_value<std::string>("weights"));
        auto W = net.at(0)->get_weights();

        for (int i = 0; i < W.rows(); ++i)
        {
            for (int j = 0; j < W.cols(); ++j)
            {
                wght << W(i, j) << " ";
            }
            wght << "\n";
        }
        wght.close();
    }
    
    return 0;
}


void config_info()
{
    exit(0);
}

void complain(const std::string &complaint)
{
    std::cerr << "Error: " << complaint << std::endl;
    exit(1);
}

const std::string timestamp(void)  
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}
// #include "Base"
// #include "include/parser.hh"
// #include "weighting.hh"
// #include <time.h>

// const std::string timestamp(void);

// template <class T, class U>
// std::ostream& operator << ( std::ostream& o, const std::map<T, U> &m)
// {
//     for (auto &e : m)
//     {
//         o << e.first << ":\n" << e.second << std::endl;
//     }
//     return o;
// }


// void complain(const std::string &complaint);

// //----------------------------------------------------------------------------
// int main(int argc, char const *argv[])
// {

//     optionparser::parser p("A CLI for retraining GAIA.");
// //----------------------------------------------------------------------------
//     p.add_option("--file", "-f")    .help("Pass at least one file to add to a TChain for testing.")
//                                     .mode(optionparser::store_mult_values);
//     //----------------------------------------------------------------------------
//     p.add_option("--tree", "-t")    .help("Name of the TTree to extract.")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("--save", "-s")    .help("Name of file to save the YAML neural network file to.")
//                                     .mode(optionparser::store_value)
//                                     .default_value(std::string("neural_net" + timestamp() + ".yaml"));
//     //----------------------------------------------------------------------------
//     p.add_option("--load")          .help("Name of a YAML neural network file to load.")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("--config", "-c")  .help("Branch config file")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("--learning")      .help("Pass a learning rate.")
//                                     .mode(optionparser::store_value)
//                                     .default_value(0.1);
//     //----------------------------------------------------------------------------
//     p.add_option("--momentum")      .help("Pass a momentum value.")
//                                     .mode(optionparser::store_value)
//                                     .default_value(0.5);
//     //----------------------------------------------------------------------------
//     p.add_option("--regularize")    .help("Pass an l2 norm regularizer.")
//                                     .mode(optionparser::store_value)
//                                     .default_value(0.00001);
//     p.add_option("--batch")         .help("Mini-batch size.")
//                                     .mode(optionparser::store_value)
//                                     .default_value(1);
//     //----------------------------------------------------------------------------
//     p.add_option("--load")          .help("Name of a YAML neural network file to load to begin training")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("--config", "-c")  .help("pass a config file")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("--struct")        .help("pass the network structure")
//                                     .mode(optionparser::store_mult_values);
//     //----------------------------------------------------------------------------
//     p.add_option("--deepauto", "-d").help("how many autoencoders to train?")
//                                     .mode(optionparser::store_value)
//                                     .default_value(-1);
//     //----------------------------------------------------------------------------
//     p.add_option("--type", "-T")    .help("Can be one of 'regress', 'multiclass', or 'binary'.")
//                                     .mode(optionparser::store_value)
//                                     .default_value("regress");
//     //----------------------------------------------------------------------------
//     p.add_option("--verbose", "-v") .help("Make the output verbose");
//     //----------------------------------------------------------------------------
//     p.add_option("--weights", "-w") .help("print a file with the first layer weight matrix.")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("-start")          .help("Start index for training. (Default = 0)")
//                                     .mode(optionparser::store_value)
//                                     .default_value(0);
//     //----------------------------------------------------------------------------
//     p.add_option("-end")            .help("End index for training. (Default, whole tree)")
//                                     .mode(optionparser::store_value)
//                                     .default_value(-1);  
//     //----------------------------------------------------------------------------
//     p.add_option("-uepochs")        .help("Number of passes over the Trees for Unsupervised Pretraining(Default = 5)")
//                                     .mode(optionparser::store_value)
//                                     .default_value(5);
//     //----------------------------------------------------------------------------
//     p.add_option("-sepochs")        .help("Number of passes over the Trees for Supervised Training(Default = 10)")
//                                     .mode(optionparser::store_value)
//                                     .default_value(10);
//     //----------------------------------------------------------------------------
//     p.add_option("--formula", "-F") .help("Specify Model Formula")
//                                     .mode(optionparser::store_value);
//     //----------------------------------------------------------------------------
//     p.add_option("-start")          .help("Start index for testing/training. (Default = 0)")
//                                     .mode(optionparser::store_value)
//                                     .default_value(0);
//     //----------------------------------------------------------------------------
//     p.add_option("-end")            .help("End index for testing/training. (Default, whole tree)")
//                                     .mode(optionparser::store_value)
//                                     .default_value(-1);
//     //----------------------------------------------------------------------------
//     p.add_option("--predict")       .help("Prediction mode!");
// //----------------------------------------------------------------------------

//     p.eat_arguments(argc, argv);


//     if (!p.get_value("file")) complain("need to pass at least one file.");
//     if (!p.get_value("tree")) complain("need to pass a tree name.");
//     if (!p.get_value("config")) complain("need a config file for variable specification.");
//     if (!p.get_value("formula")) complain("need a model formula to train.");
//     if (!p.get_value("struct")) complain("need to pass a network structure.");
//     std::vector<std::string> root_files(p.get_value<std::vector<std::string>>("file"));

//     std::string ttree_name =    p.get_value<std::string>("tree"),
//                 config_file =   p.get_value<std::string>("config"),
//                 save_file =     p.get_value<std::string>("save"),
//                 model_formula = p.get_value<std::string>("formula");




//     double learning =    p.get_value<double>("learning"), 
//            momentum =    p.get_value<double>("momentum"),
//            regularizer = p.get_value<double>("regularize");

//     int deepauto =    p.get_value<int>("deepauto"),
//         start =       p.get_value<int>("start"),
//         end =         p.get_value<int>("end"),
//         uepochs =     p.get_value<int>("uepochs"),
//         sepochs =     p.get_value<int>("sepochs"),
//         batch =       p.get_value<int>("batch");
//     bool verbose = p.get_value("verbose");

//     std::vector<int> structure = p.get_value<std::vector<int>>("struct");

//     if (deepauto < 0)
//     {
//         deepauto = structure.size();
//     }

//     agile::root::tree_reader TR;


//     for (auto &file : root_files)
//     {
//        TR.add_file(file, ttree_name);
//     }

//     TR.set_branches(config_file);

// //----------------------------------------------------------------------------
//     agile::dataframe D = TR.get_dataframe(end - start, start, verbose);

//     std::ofstream dframe("testfram.csv");
   
//     dframe << D;

//     dframe.close();

//     agile::neural_net net;
//     net.add_data(D);

//     layer_type net_type;
//     std::string passed_target = p.get_value<std::string>("type");

// //----------------------------------------------------------------------------

//     if (passed_target == "regress") net_type = linear;
//     else if (passed_target == "multiclass") net_type = softmax;
//     else if (passed_target == "binary") net_type = sigmoid;
//     else complain(
//         "type of target needs to be one of 'regress', 'multiclass', or 'binary'.");
    
// //----------------------------------------------------------------------------

//     int i;
//     for (i = 0; i < (structure.size() - 2); ++i)
//     {
//         if (i < deepauto)
//         {
//             net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
//         }
//         else
//         {
//             net.emplace_back(new layer(structure[i], structure[i + 1], sigmoid));
//         }
        
//     }
//     if (i < deepauto)
//     {
//         net.emplace_back(new autoencoder(structure[i], structure[i + 1], net_type));
//     }
//     else
//     {
//         net.emplace_back(new layer(structure[i], structure[i + 1], net_type));
//     }
    
//     if (verbose)
//     {
//         std::cout << "\nParsing model formula " << model_formula << "...";
//     }
//     net.model_formula(model_formula, true, verbose);
//     if (verbose)
//     {
//         std::cout << "Done." << std::endl;
//     }

//     net.set_learning(learning);
//     net.set_regularizer(regularizer);
//     net.set_momentum(momentum);
//     net.set_batch_size(batch);
    
//     net.check(0);

//     if (verbose)
//     {
//         std::cout << "Performing Unsupervised Pretraining...";
//     }
//     net.train_unsupervised(uepochs, verbose);
//     if (verbose)
//     {
//         std::cout << "\nPerforming Supervised Training...\n";
//     }
//     net.train_supervised(sepochs, verbose);
//     if (verbose)
//     {
//         std::cout << "\nDone.\nSaving to " << save_file << "...";
//     }
//     net.to_yaml(save_file, TR.get_var_types(), TR.get_binning());
//     if (verbose)
//     {
//         std::cout << "Done." << std::endl;
//     }
// //----------------------------------------------------------------------------

// //     agile::root::tree_reader tree_buf;

// //     for (auto &file : root_files) tree_buf.add_file(file, ttree_name);
// //     tree_buf.set_branches(config_file);



// // // Reweighting
// // //----------------------------------------------------------------------------
// //     std::cout << "Determining Reweighting..." << std::endl;
// //     agile::root::weighting jet_weights;
// //     jet_weights.light_percentage(0.54)
// //                .charm_percentage(0.11)
// //                .bottom_percentage(0.35)
// //                .gen_hist(tree_buf, 200000, 0, verbose);
// // //----------------------------------------------------------------------------
// // // 
// //     // agile::dataframe D((std::move(
// //         // tree_buf.get_dataframe(jet_weights, end - start, start, verbose))));
// //     agile::dataframe D((std::move(
// //         tree_buf.get_dataframe(end - start, start, verbose))));

// //     // std::ofstream dframe("testfram.csv");
   
// //     // dframe << D;

// //     // dframe.close();

// // //----------------------------------------------------------------------------
    
// //     agile::neural_net net;
// //     // net.load_model_frame_config(frame);
// //     net.add_data(D);

// // //----------------------------------------------------------------------------

// //     layer_type net_type;
// //     std::string passed_target = p.get_value<std::string>("type");


// // //----------------------------------------------------------------------------
// //     if (passed_target == "regress") net_type = linear;
// //     else if (passed_target == "multiclass") net_type = softmax;
// //     else if (passed_target == "binary") net_type = sigmoid;
// //     else complain(
// //         "type of target needs to be one of 'regress', 'multiclass', or 'binary'.");
    
// // //----------------------------------------------------------------------------

// //     int i;
// //     for (i = 0; i < (structure.size() - 2); ++i)
// //     {
// //         if (i < deepauto)
// //         {
// //             net.emplace_back(new autoencoder(structure[i], structure[i + 1], sigmoid));
// //         }
// //         else
// //         {
// //             net.emplace_back(new layer(structure[i], structure[i + 1], sigmoid));
// //         }
        
// //     }
// //     if (i < deepauto)
// //     {
// //         net.emplace_back(new autoencoder(structure[i], structure[i + 1], net_type));
// //     }
// //     else
// //     {
// //         net.emplace_back(new layer(structure[i], structure[i + 1], net_type));
// //     }

// //     net.model_formula(model_formula, true, verbose);

// //     net.set_learning(learning);
// //     net.set_regularizer(regularizer);
// //     net.set_momentum(momentum);
// //     net.set_batch_size(batch);

// //     net.check(0);


// // // Training
// // //----------------------------------------------------------------------------

// //     if (verbose)
// //     {
// //         std::cout << "Performing Unsupervised Pretraining...";
// //     }
// //     net.train_unsupervised(uepochs, verbose);
// //     if (verbose)
// //     {
// //         std::cout << "\nPerforming Supervised Training...\n";
// //     }
// //     net.train_supervised(sepochs, verbose);
// //     if (verbose)
// //     {
// //         std::cout << "\nDone.\nSaving to " << save_file << "...";
// //     }
// //     net.to_yaml(save_file, tree_buf.get_var_types(), 
// //         tree_buf.get_binning(), tree_buf.get_constraints());



// //     // net.from_yaml(load_file);

// //     // std::vector<std::string> input_vars {"pt", "bottom", "charm", "light", "MV1"};


// //     // std::cout << "prob_bottom, bottom, charm, light, pt, MV1" << std::endl;
// //     // for (int i = start; i < end; ++i)
// //     // {
// //     //     auto pred = net.predict_map(TR(i, net.get_inputs()));

// //     //     auto control = TR(i, input_vars);

// //     //     std::cout << pred["bottom"] << "," << control["bottom"] << "," << control["charm"] << "," << control["light"] << "," << control["pt"] << "," << control["MV1"] << std::endl;
// //     // }



//     return 0;
// }


// void complain(const std::string &complaint)
// {
//     std::cerr << "Error: " << complaint << std::endl;
//     exit(1);
// }

// const std::string timestamp(void)  
// {
//     time_t now = time(0);
//     struct tm tstruct;
//     char buf[80];
//     tstruct = *localtime(&now);
//     strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//     return buf;
// }