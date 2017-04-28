/*!
  @file   XGBooster.cpp
  @author David Hirvonen
  @brief  Internal implementation of the XGBoost C++ interface class.

  \copyright Copyright 2014-2016 Elucideye, Inc. All rights reserved.
  \license{This project is released under the 3 Clause BSD License.}

*/

#include "xgboostcpp/XGBooster.h"
#include "xgboostcpp/XGBoosterImpl.h"

XGBOOSTCPP_NAMESPACE_BEGIN

XGBooster::XGBooster()
{
    m_impl = std::make_shared<XGBooster::Impl>();
}

XGBooster::XGBooster(const Recipe &recipe)
{
    m_impl = std::make_shared<XGBooster::Impl>(recipe);
}

float XGBooster::operator()(const std::vector<float> &features)
{
    return (*m_impl)(features);
}

void XGBooster::train(const MatrixType<float> &features, const std::vector<float> &values, const MatrixType<uint8_t> &mask)
{
    m_impl->train(features, values, mask);
}

void XGBooster::read(const std::string &filename)
{
    m_impl->read(filename);
}
void XGBooster::write(const std::string &filename) const
{
    m_impl->write(filename);
}

XGBOOSTCPP_NAMESPACE_END

