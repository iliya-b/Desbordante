#include "tane.h"

#include "common_tane.h"
#include "config/error/option.h"
#include "config/max_lhs/option.h"

namespace algos {

Tane::Tane() : tane::CommonTane() {
    RegisterOptions();
}

void Tane::RegisterOptions() {
    RegisterOption(config::ErrorOpt(&max_ucc_error_));
    RegisterOption(config::MaxLhsOpt(&max_lhs_));
}

void Tane::MakeExecuteOptsAvailable() {
    MakeOptionsAvailable({config::MaxLhsOpt.GetName(), config::ErrorOpt.GetName()});
}

double Tane::CalculateZeroAryFdError(ColumnData const* rhs) {
    return 1 - rhs->GetPositionListIndex()->GetNepAsLong() /
                       static_cast<double>(GetRelation().GetNumTuplePairs());
}

double Tane::CalculateFdError(model::PositionListIndex const* lhs_pli,
                              model::PositionListIndex const* joint_pli) {
    return (double)(lhs_pli->GetNepAsLong() - joint_pli->GetNepAsLong()) /
           static_cast<double>(GetRelation().GetNumTuplePairs());
}

}  // namespace algos