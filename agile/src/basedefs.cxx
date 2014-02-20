#include "agile/include/basedefs.hh"

//----------------------------------------------------------------------------
std::mt19937_64& agile::mersenne_engine() // dumb random number generator
{
    static std::mt19937_64 _eng{};
    return _eng;
}
//----------------------------------------------------------------------------
std::string agile::stringify(const agile::matrix &M)
{
    std::stringstream ss;
    ss << "#EM|" << M.rows() << "|" << M.cols() << std::setprecision(15);
    for (int i = 0; i < M.rows(); ++i)
    {
        for (int j = 0; j < M.cols(); ++j)
        {
            ss << "," << M(i, j);
        }
    }
    return ss.str();
}
//----------------------------------------------------------------------------
agile::matrix agile::destringify(const std::string &s)
{
    auto t(s);
    auto first_bar = s.find_first_of("|");
    auto last_bar = s.find_last_of("|");
    auto found = s.find_first_of(",");
    int rows = std::stoi(s.substr(first_bar + 1, last_bar - first_bar - 1));
    int cols = std::stoi(s.substr(last_bar + 1, found - last_bar - 1));
    
    t.erase(0, found + 1);

    agile::matrix M(rows, cols);
    std::istringstream iss( t );
    std::vector<double> data;

    while (std::getline(iss, t, ',')) 
    {
        double fieldvalue = 0.000;
        std::istringstream( t ) >> fieldvalue;
        data.push_back( fieldvalue );
    }
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            M(i, j) = data.at(i * cols + j);
        }
    }
    return std::move(M);
}
//----------------------------------------------------------------------------
agile::vector agile::std_to_Eigen(std::vector<numeric> &v)
{
    numeric *ptr = &v[0];
    std_2_eigen _tmp(ptr, v.size());
    return std::move(_tmp);
}
//----------------------------------------------------------------------------
void agile::progress_bar(int percent) 
{
    std::string _prog_bar, this_color;
    if (percent < 34)
    {
        this_color = agile::colors::red();
    }
    else if (percent < 67)
    {
        this_color = agile::colors::yellow();
    }
    else
    {
        this_color = agile::colors::green();
    }
    for(int i = 0; i < 50; i++) 
    {
        if (i < (percent/2)) 
        {
            _prog_bar.replace(i,1,"=");
        }
        else if (i == (percent/2)) 
        {
            _prog_bar.replace(i,1,">");
        }
        else 
        {
            _prog_bar.replace(i,1," ");
        }
    }
    std::cout<< "\r" "[" << this_color << _prog_bar << agile::colors::reset() << "] ";
    std::cout.width( 3 );
    std::cout<< percent << "%     " << std::flush;
}
