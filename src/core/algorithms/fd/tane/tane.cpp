#include "tane.h"

namespace algos {

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
