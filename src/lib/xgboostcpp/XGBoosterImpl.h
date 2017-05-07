/*!
 @file   XGBoosterImpl.h
 @author David Hirvonen
 @brief  Internal declaration of the XGBoost private implementation class.
 
 \copyright Copyright 2014-2016 Elucideye, Inc. All rights reserved.
 \license{This project is released under the 3 Clause BSD License.}
 
 */

#ifndef __xgboostcpp_XGBoosterImpl_h__
#define __xgboostcpp_XGBoosterImpl_h__

#include "xgboostcpp/Booster.h"

XGBOOSTCPP_BEGIN_NAMESPACE(xgboostcpp)

template <typename T> std::string xtos(const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

class XGBooster::Impl
{
public:

    typedef XGBooster::Recipe Recipe;

    Impl()
    {
        m_recipe.numberOfTrees = 512;
        m_recipe.maxDepth = 4;
        m_recipe.learningRate = 0.1;
        m_recipe.dataSubsample = 0.5;
        m_recipe.regression = true;

        init();
    }
    Impl(const Recipe &recipe) : m_recipe(recipe)
    {
        init();
    }

    ~Impl() {}
    
    void init()
    {
        m_booster = std::make_shared<xgboost::wrapper::Booster>();

        //xgboost::wrapper::Booster tsk(dmats);

        // https://github.com/dmlc/xgboost/blob/master/doc/parameter.md
        m_booster->SetParam("silent", "1");
        m_booster->SetParam("nthread", "8");
        m_booster->SetParam("booster", "gbtree");

        m_booster->SetParam("eta", xtos(m_recipe.learningRate).c_str()); // shrinkage
        m_booster->SetParam("max_depth", xtos(m_recipe.maxDepth).c_str());
        m_booster->SetParam("subsample", xtos(m_recipe.dataSubsample).c_str());
        m_booster->SetParam("colsample_bytree", xtos(m_recipe.featureSubsample).c_str());

        m_booster->SetParam("gamma", "0");
        m_booster->SetParam("min_child_weight", "1"); // TODO (1 == dflt)
        m_booster->SetParam("max_delta_step", "0");   // TODO (1 == dflt)

        if(m_recipe.regression)
        {
            m_booster->SetParam("objective", "reg:linear"); // linear regression (vs. logistic)
        }
        else
        {
            m_booster->SetParam("objective", "binary:logistic");
        }
    }

    float operator()(const std::vector<float> &features)
    {
        std::shared_ptr<DMatrixSimple> dTest = xgboost::DMatrixSimpleFromMat(&features[0], 1, features.size(),NAN);
        std::vector<float> predictions(1, 0.f);
        m_booster->Predict(*dTest, false, &predictions);
        return predictions.front();
    }

    void train(const MatrixType<float> &features, const std::vector<float> &values, const MatrixType<uint8_t> &mask= {})
    {
        std::shared_ptr<DMatrixSimple> dTrain = xgboost::DMatrixSimpleFromMat(features, features.size(), features[0].size(), mask);
        dTrain->info.labels = values;

        std::vector< xgboost::learner::DMatrix *> dmats { dTrain.get() };
        m_booster->SetCacheData(dmats);
        m_booster->CheckInitModel();
        m_booster->CheckInit(dTrain.get());

        //std::vector<DataMatrix *> dmats { dTrain.get() }; // what is this for?

        for(int t = 0; t < m_recipe.numberOfTrees; t++)
        {
            m_booster->UpdateOneIter(t, *dTrain);
        }
    }

    void read(const std::string &name)
    {
        // normal XGBoost logging not needed with boost serialization
        m_booster->LoadModel(name.c_str());
    }

    void write(const std::string &name)
    {
        m_booster->SaveModel(name.c_str(), true); // with_pbuffer TODO
    }

    template<class Archive> void serialize(Archive & ar, const unsigned int version)
    {
        ar & m_recipe;
        ar & m_booster;
    }

protected:

    Recipe m_recipe;
    std::shared_ptr<xgboost::wrapper::Booster> m_booster;
};

// #################################################################################
// #########                      XGBooster                                #########
// #################################################################################

template<class Archive> void XGBooster::Recipe::serialize(Archive & ar, const unsigned int version)
{
    ar & numberOfTrees;
    ar & maxDepth;
    ar & dataSubsample;
    ar & learningRate;
    if(version >= 1)
    {
        ar & featureSubsample;
    }
}

// Boost serialization:
template<class Archive> void XGBooster::serialize(Archive & ar, const unsigned int version)
{
    ar & m_impl;
}

XGBOOSTCPP_END_NAMESPACE(xgboostcpp)

#endif // __xgboostcpp_XGBoosterImpl_h__
