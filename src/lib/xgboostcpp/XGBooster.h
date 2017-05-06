/*!
  @file   Xgboostcpp.h
  @author David Hirvonen
  @brief  Internal declaration of the XGBoost C++ interface class.

  \copyright Copyright 2014-2016 Elucideye, Inc. All rights reserved.
  \license{This project is released under the 3 Clause BSD License.}

*/

#ifndef __xgboostcpp_XGBooster_h__
#define __xgboostcpp_XGBooster_h__

#include "xgboostcpp/xgboostcpp_def.h"

#if defined(XGBOOST_USE_BOOST)
#  include <boost/serialization/export.hpp>
#endif

#include <vector>
#include <memory>

template <typename T>
using MatrixType = std::vector<std::vector<T>>;

XGBOOSTCPP_BEGIN_NAMESPACE(xgboostcpp)

class XGBooster
{
public:

    struct Recipe
    {
        int numberOfTrees = 512;
        int maxDepth = 5;
        double dataSubsample = 0.5;
        double learningRate = 0.1;
        double featureSubsample = 0.1;
        bool regression = true;

        template<class Archive> void serialize(Archive & ar, const unsigned int version);
    };

    class Impl;
    XGBooster();
    XGBooster(const Recipe &recipe);
    float operator()(const std::vector<float> &features);
    void train(const MatrixType<float> &features, const std::vector<float> &values, const MatrixType<uint8_t> &mask={});

    void read(const std::string &filename);
    void write(const std::string &filename) const;

    // Boost serialization:
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

protected:

    std::shared_ptr<Impl> m_impl;
};

XGBOOSTCPP_END_NAMESPACE(xgboostcpp)

#if XGBOOSTCPP_SERIALIZE_WITH_BOOST
BOOST_CLASS_EXPORT_KEY(xgboostcpp::XGBooster);
BOOST_CLASS_EXPORT_KEY(xgboostcpp::XGBooster::Impl);
#endif

#endif // __xgboostcpp_XGBooster_h__
