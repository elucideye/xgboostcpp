#include "xgboostcpp/XGBooster.h"

#include "cxxopts.hpp"

#include <iostream>
#include <fstream>

int xgboost_test(const std::string &input, const std::string &model, std::ostream &output);
int xgboost_train(const std::string &input, const std::string &model, xgboostcpp::XGBooster::Recipe &params);

int main(int argc, char **argv)
{
    const auto argumentCount = argc;

    // ############################
    // ### Command line parsing ###
    // ############################
    
    std::string sInput, sModel, sOutput;
    bool doTrain = false;
    
    cxxopts::Options options("test-xgboostcpp", "Command line interface for xgboostcpp.");
    options.add_options()
        ("i,input", "Input file", cxxopts::value<std::string>(sInput))
        ("o,output", "Output directory", cxxopts::value<std::string>(sOutput))
        ("m,model", "Input file", cxxopts::value<std::string>(sModel))
        ("t,train", "Training mode.", cxxopts::value<bool>(doTrain))
        ("h,help", "Print help message");
    
    options.parse(argc, argv);
    
    if((argumentCount <= 1) || options.count("help"))
    {
        std::cerr << options.help({""}) << std::endl;
        return 0;
    }
    
    if(sModel.empty())
    {
        std::cerr << "Must specify valid model file." << std::endl;
        return -1;
    }
    
    if(sInput.empty())
    {
        std::cerr << "Must specify input CSV file." << std::endl;
        return -1;
    }

    if(doTrain)
    {
        xgboostcpp::XGBooster::Recipe params; // default....
        // TODO: param parsing (use cereal json file)
        return xgboost_train(sInput, sModel, params);
    }
    else
    {
        std::ostream* os = &std::cout;
        std::ofstream ofs;
        if (!sOutput.empty())
        {
            ofs.open(sOutput);
            os = &ofs;
        }

        return xgboost_test(sInput, sModel, *os);
    }
    return -1;
}


#include <boost/spirit/include/qi.hpp>
#include <iterator>

namespace qi = boost::spirit::qi;

using Vector32f = std::vector<float>;
using Matrix32f = std::vector<std::vector<float>>;

// http://stackoverflow.com/a/1764367
template <typename Iterator>
bool loadCSV(Iterator first, Iterator last, Matrix32f &v, Vector32f &x)
{
    bool r = boost::spirit::qi::phrase_parse(first, last, +(qi::float_ % ','), qi::space, v);
    if(r)
    {
        x.resize(v.size());
        for(int i = 0; i < v.size(); i++)
        {
            x[i] = v[i].back();
            v[i].pop_back();
        }
    }
    return r;
}

static bool loadCSV(std::istream &is, Matrix32f &v, Vector32f &x)
{
    bool r = false;
    if(is)
    {
        // wrap istream into iterator
        is.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator first(is), last;
        r = loadCSV(first, last, v, x);
    }
    return r;
}

static bool loadCSV(const std::string &filename, Matrix32f &v, Vector32f &x)
{
    std::ifstream is(filename);
    return loadCSV(is, v, x);
}

// ### pba.z ###

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include "boost-pba/portable_binary_oarchive.hpp"
#include "boost-pba/portable_binary_iarchive.hpp"

template <typename T>
void load_pba_z(std::istream &is, T &object)
{
    boost::iostreams::filtering_stream<boost::iostreams::input> buffer;
    buffer.push(boost::iostreams::zlib_decompressor());
    buffer.push(is);
    portable_binary_iarchive ia(buffer);
    ia >> object;
}

template <typename T>
void load_pba_z(const std::string &filename, T &object)
{
    std::ifstream ifs(filename, std::ios::binary);
    assert(ifs); // TODO: throw
    load_pba_z(ifs, object);
}

template <typename T>
void save_pba_z(std::ostream &os, T &object)
{
    boost::iostreams::filtering_stream<boost::iostreams::output> buffer;
    buffer.push(boost::iostreams::zlib_compressor(boost::iostreams::zlib::best_compression));
    buffer.push(os);
    portable_binary_oarchive oa(buffer);
    oa << object;
}

template <typename T>
void save_pba_z(const std::string &filename, T &object)
{
    std::ofstream ofs(filename, std::ios::binary);
    assert(ofs); // TODO: throw
    save_pba_z(ofs, object);
}

int xgboost_test(const std::string &input, const std::string &model, std::ostream &os)
{
    Matrix32f x;
    Vector32f y;
    if(!loadCSV(input, x, y))
    {
        return -1;
    }

    xgboostcpp::XGBooster xgb;
    load_pba_z(model, xgb);

    Vector32f predictions(x.size());
    for(int i = 0; i < x.size(); i++)
    {
        predictions[i] = xgb(x[i]);
    }
    std::copy(predictions.begin(), predictions.end(), std::ostream_iterator<float>(os, "\n"));
    
    return 0;
}

int xgboost_train(const std::string &input, const std::string &model, xgboostcpp::XGBooster::Recipe &params)
{
    Matrix32f x;
    Vector32f y;
    if(!loadCSV(input, x, y))
    {
        return -1;
    } 
    xgboostcpp::XGBooster xgb(params);
    xgb.train(x, y);
    save_pba_z(model, xgb);

    return 0;
}
