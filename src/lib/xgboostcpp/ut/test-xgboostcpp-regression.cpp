#include <gtest/gtest.h>

#include "xgboostcpp/XGBooster.h"
#include "xgboostcpp/xgboostcpp_serialize.h"

#include "boston_train.h"
#include "boston_test.h"

#include <algorithm>
#include <cmath>

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

static std::shared_ptr<xgboostcpp::XGBooster> create()
{
    xgboostcpp::XGBooster::Recipe recipe;
    
    // {'n_estimators': 500, 'max_depth': 4, 'min_samples_split': 2, 'learning_rate': 0.01, 'loss': 'ls'}
    recipe.numberOfTrees = 500;
    recipe.maxDepth = 4;
    recipe.learningRate = 0.01;
    recipe.dataSubsample = 0.75;     // some value < 1
    recipe.featureSubsample = 0.75;  // -/-
    
    return std::make_shared <xgboostcpp::XGBooster>(recipe);
}

using Vector32f = std::vector<float>;
using Matrix32f = std::vector<Vector32f>;

struct FeaturesLabel
{
    FeaturesLabel(const Matrix32f &data)
    {
        x = data;
        y.resize(x.size());
        for(int i = 0; i < y.size(); i++)
        {
            y[i] = x[i].back();
            x[i].pop_back();
        }
    }
    
    Matrix32f x;
    Vector32f y;
};

struct TestHarness
{
    using XGBoosterPtr = std::shared_ptr<xgboostcpp::XGBooster>;
    using ReadFunc = std::function<void(XGBoosterPtr &)>;
    using WriteFunc = std::function<void(XGBoosterPtr &)>;
    
    TestHarness() {}
    
    float operator()()
    {
        auto xgb = create();
        
        // ::: Training :::
        FeaturesLabel train(get_boston_train());
        xgb->train(train.x, train.y);
        
        if(reader && writer)
        {
            // Serialization test:
            writer(xgb);
            xgb = nullptr;
            reader(xgb);
        }
        
        // ::: Testing :::
        float e2 = 0.f;
        FeaturesLabel test(get_boston_test());
        for(int i = 0; i < test.x.size(); i++)
        {
            // predict:
            const float y = (*xgb)(test.x[i]);
            
            //std::cout << y << " vs " << test.y[i] << std::endl;
            
            // rmse:
            e2 += std::pow(y - test.y[i], 2.f);
        }
        
        float mse = e2 / static_cast<float>(test.y.size());
      
        return mse;
    }
        
    ReadFunc reader;
    WriteFunc writer;
};
    
// http://scikit-learn.org/stable/auto_examples/ensemble/plot_gradient_boosting_regression.html
TEST(XGBoostCPPTest, BostonHousingRegression)
{
    TestHarness tester;
    auto mse = tester();
    std::cout << "MSE: " << mse << std::endl;
    ASSERT_LT(tester(), 6.6213);
}

// ##############
// ### CEREAL ###
// ##############

#if defined(XGBOOSTER_SERIALIZE_WITH_CEREAL)

TEST(XGBoostCPPTest, BostonHousingRegressionCereal)
{
    TestHarness tester;
    
    std::string filename = "xgb.cpb";
    
    tester.writer = [&](std::shared_ptr<xgboostcpp::XGBooster> &xgb)
    {
        save_cpb(filename, xgb);
    };
    
    tester.reader = [&](std::shared_ptr<xgboostcpp::XGBooster> &xgb)
    {
        load_cpb(filename, xgb);
    };

    auto mse = tester();
    std::cout << "MSE (cereal): " << mse << std::endl;
    ASSERT_LT(mse, 6.6213); // compare to sci-kit example 6.6213
}

#endif // XGBOOSTER_SERIALIZE_WITH_CEREAL


