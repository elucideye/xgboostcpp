#include "xgboostcpp/to_string.h" // for android std additions

#include "xgboostcpp/XGBooster.h"
#include "xgboostcpp/Booster.h"
#include "xgboostcpp/XGBoosterImpl.h"

// include all std functions
using namespace std;
#include "xgboost/wrapper/xgboost_wrapper.h"
#include "xgboost/src/gbm/gbm.h"
#include "xgboost/src/gbm/gbtree-inl.hpp"
#include "xgboost/src/gbm/gblinear-inl.hpp"
#include "xgboost/src/data.h"
#include "xgboost/src/learner/learner-inl.hpp"
#include "xgboost/src/io/io.h"
#include "xgboost/src/utils/utils.h"
#include "xgboost/src/utils/math.h"
#include "xgboost/src/utils/group_data.h"
#include "xgboost/src/io/simple_dmatrix-inl.hpp"

#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/portable_binary.hpp>

using TreeModel32f = xgboost::tree::TreeModel<bst_float, xgboost::tree::RTreeNodeStat>;

// Tree model:
using RTreeNodeStat = xgboost::tree::RTreeNodeStat;
using TreeModel =  xgboost::tree::TreeModel<bst_float, RTreeNodeStat>;

// ##################################################################
// #################### portable_binary_*archive ####################
// ##################################################################

typedef cereal::PortableBinaryOutputArchive OArchive;
typedef cereal::PortableBinaryInputArchive IArchive;

XGBOOSTCPP_NAMESPACE_BEGIN
template void XGBooster::serialize<OArchive>(OArchive &ar, const unsigned int);
template void XGBooster::Recipe::serialize<OArchive>(OArchive &ar, const unsigned int);
template void XGBooster::Impl::serialize<OArchive>(OArchive &ar, const unsigned int);

template void XGBooster::serialize<IArchive>(IArchive &ar, const unsigned int);
template void XGBooster::Recipe::serialize<IArchive>(IArchive &ar, const unsigned int);
template void XGBooster::Impl::serialize<IArchive>(IArchive &ar, const unsigned int);
XGBOOSTCPP_NAMESPACE_END

template void xgboost::gbm::serialize(OArchive & ar, xgboost::gbm::IGradBooster &gb, const unsigned int version);
template void xgboost::gbm::serialize(OArchive & ar, xgboost::gbm::GBTree &gbt, const unsigned int version);
template void xgboost::gbm::serialize(OArchive & ar, xgboost::gbm::GBLinear &gbt, const unsigned int version);

template void xgboost::gbm::serialize(IArchive & ar, xgboost::gbm::IGradBooster &gb, const unsigned int version);
template void xgboost::gbm::serialize(IArchive & ar, xgboost::gbm::GBTree &gbt, const unsigned int version);
template void xgboost::gbm::serialize(IArchive & ar, xgboost::gbm::GBLinear &gbt, const unsigned int version);
