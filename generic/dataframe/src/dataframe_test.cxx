#include "dataframe.hh"
#include <iostream>


void parse_formula(std::string formula)
{
    formula = agile::no_spaces(formula);
    bool parsing = true, wildcard = false;
    auto tilde = formula.find_first_of("~");
    if (tilde != formula.find_last_of("~"))
    {
        throw std::runtime_error("can't specify multiple \'is a function of\' operators (uses of \'~\') in one formula.");
    }

    auto wildcard_find = formula.find_first_of("*");
    if (wildcard_find != formula.find_last_of("*"))
    {
        throw std::runtime_error("can't specify multiple \'include all defined variables\' operators (uses of \'*\') in one formula.");
    }
    
    if (wildcard_find != std::string::npos)
    {
        wildcard = true;
        std::string::iterator end_pos = std::remove(formula.begin(), formula.end(), '*');
        formula.erase(end_pos, formula.end());
    }
    auto lhs = formula.substr(0, tilde);
    auto rhs = formula.substr(tilde + 1);
    auto end = lhs.find_first_of('+');
    unsigned long int start = 0;
    while(parsing == true)
    {
        auto new_var = lhs.substr(start, end - start);
        std::cout << new_var << std::endl;   
        if (end == std::string::npos) parsing = false;
        start = end + 1;
        end = lhs.find_first_of('+', start);
    }
    parsing = true;
    start = (rhs[0] == '-' || rhs[0] == '+') ? 1 : 0;
    end = rhs.find_first_of("+-", start);
    while(parsing == true)
    {
        auto new_var = rhs.substr(start, end - start);
        if (rhs[start - 1] == '-')
        {
            std::cout << "removing: ";
        }
        std::cout << new_var << std::endl;
        if (end == std::string::npos) parsing = false;
        start = end + 1;
        end = rhs.find_first_of("+-", start);
    }
    if (wildcard)
    {
        std::cout << "This was wildcarded, so only negatives will be considered." << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    std::string formula = "y1 + y2+y_3+ y4~ * -x1+ xblah - *z2 + x_2 + x3";

    parse_formula(formula);

    // std::cout << "hi" << std::endl;
    // auto csv_file = std::string(argv[1]);
    // agile::dataframe D(csv_file, true);
    // std::cout << "There are " << D.rows() << " rows and " << D.columns() << " columns." << std::endl;

    // std::cout << "the column names are:\n";
    // for (auto &entry : D.get_column_names())
    // {
    //     std::cout << entry << "   ";
    // }
    // std::cout << "\nThe second row is:\n";
    
    // std::vector<double> v = D.at(1);
    // for (auto &entry : v)
    // {
    //     std::cout << entry << "   ";
    // }
    // std::cout << "\nD.at(1, col2) = " << D.at(1, "col2") << std::endl;

    // D.push_back({1.1, 3.54, 0.9999});

    // D.to_csv("outcsv.csv");

    // std::cout << "new set:\n" << D << "\n" << std::endl;


    // agile::dataframe F("outcsv.csv", true);

    // D.append(std::move(F));


    // std::cout << "appended set:\n" << D << "\n" << std::endl;

    // std::cout << "\nD.at(1, mycol) = " << D.at(1, "mycol") << std::endl;
    return 0;
}