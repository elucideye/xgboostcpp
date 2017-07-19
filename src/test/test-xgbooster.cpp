#include "xgboostcpp/XGBooster.h"
#include "xgboostcpp/xgboostcpp_serialize.h"

#include "cxxopts.hpp"

#include <iostream>
#include <fstream>

#include <assert.h>

int xgboost_test(const std::string &input, const std::string &model, std::ostream &output);
int xgboost_train(const std::string &input, const std::string &model, xgboostcpp::XGBooster::Recipe &params);

int main(int argc, char **argv)
{
    const auto argumentCount = argc;

    // ############################
    // ### Command line parsing ###
    // ############################
    
    std::string sInput, sModel, sOutput, sArchive;

#if !defined(XGBOOSTER_DO_LEAN)
    bool doTrain = false;
#endif

    cxxopts::Options options("test-xgboostcpp", "Command line interface for xgboostcpp.");
    options.add_options()
        ("i,input", "Input CSV file", cxxopts::value<std::string>(sInput))
        ("o,output", "Output directory", cxxopts::value<std::string>(sOutput))
        ("m,model", "Model archive: *.cpb or *.pba.z", cxxopts::value<std::string>(sModel))
#if !defined(XGBOOSTER_DO_LEAN)
        ("t,train", "Training mode.", cxxopts::value<bool>(doTrain))
#endif
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

    auto type = getType(sModel);
    assert(type != kUnknown);
    
    if(sInput.empty())
    {
        std::cerr << "Must specify input CSV file." << std::endl;
        return -1;
    }

#if !defined(XGBOOSTER_DO_LEAN)    
    if(doTrain)
    {
        xgboostcpp::XGBooster::Recipe params; // default....
        // TODO: param parsing (use cereal json file)
        return xgboost_train(sInput, sModel, params);
    }
    else
#endif
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
bool loadCSV(Iterator first, Iterator last, Matrix32f &x)
{
    return boost::spirit::qi::phrase_parse(first, last, +(qi::float_ % ','), qi::space, x);
}

static bool loadCSV(std::istream &is, Matrix32f &x)
{
    bool r = false;
    if(is)
    {
        // wrap istream into iterator
        is.unsetf(std::ios::skipws);
        boost::spirit::istream_iterator first(is), last;
        r = loadCSV(first, last, x);
    }
    return r;
}

static bool loadCSV(const std::string &filename, Matrix32f &x)
{
    std::ifstream is(filename);
    return loadCSV(is, x);
}

// http://stackoverflow.com/a/1764367
template <typename Iterator>
bool loadCSV(Iterator first, Iterator last, Matrix32f &x, Vector32f &y)
{
    bool r = boost::spirit::qi::phrase_parse(first, last, +(qi::float_ % ','), qi::space, x);
    if(r)
    {
        x.resize(y.size());
        for(int i = 0; i < x.size(); i++)
        {
            y[i] = x[i].back();
            x[i].pop_back();
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

int xgboost_test(const std::string &input, const std::string &model, std::ostream &os)
{
    Matrix32f x;
    Vector32f y;
    if(!loadCSV(input, x, y))
    {
        return -1;
    }

    xgboostcpp::XGBooster xgb;

    load_model(model, xgb);

    Vector32f predictions(x.size());
    for(int i = 0; i < x.size(); i++)
    {
        predictions[i] = xgb(x[i]);
    }
    std::copy(predictions.begin(), predictions.end(), std::ostream_iterator<float>(os, "\n"));
    
    return 0;
}

#if !defined(XGBOOSTER_DO_LEAN)
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

    save_model(model, xgb);
        
    return 0;
}
#endif
