#include "tane.h"

#include <chrono>
#include <iomanip>
#include <list>
#include <memory>

#include <easylogging++.h>

#include "config/error/option.h"
#include "config/error_measure/option.h"
#include "enums.h"
#include "fd/pli_based_fd_algorithm.h"
#include "fd/tane/model/lattice_level.h"
#include "fd/tane/model/lattice_vertex.h"
#include "model/table/column_data.h"
#include "model/table/column_layout_relation_data.h"
#include "model/table/relational_schema.h"

namespace algos {
using boost::dynamic_bitset;
using Cluster = model::PositionListIndex::Cluster;

Tane::Tane(std::optional<ColumnLayoutRelationDataManager> relation_manager)
    : tane::TaneCommon(relation_manager) {}

void Tane::MakeExecuteOptsAvailableFDInternal() {
    MakeOptionsAvailable({config::kErrorOpt.GetName()});
}

config::ErrorType Tane::CalculateZeroAryFdError(ColumnData const* rhs) {
    return 1 - rhs->GetPositionListIndex()->GetNepAsLong() /
                       static_cast<double>(relation_.get()->GetNumTuplePairs());
}

config::ErrorType Tane::CalculateFdError(model::PositionListIndex const* lhs_pli,
                                         model::PositionListIndex const* joint_pli) {
    return (double)(lhs_pli->GetNepAsLong() - joint_pli->GetNepAsLong()) /
           static_cast<double>(relation_.get()->GetNumTuplePairs());
}

}  // namespace algos
