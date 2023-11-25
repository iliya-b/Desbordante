#include "probatane.h"

#include <chrono>
#include <iomanip>
#include <list>
#include <memory>

#include <easylogging++.h>

#include "config/error/option.h"
#include "config/max_lhs/option.h"
#include "model/table/column_combination.h"
#include "model/table/column_data.h"
#include "model/table/column_layout_relation_data.h"
#include "model/table/relational_schema.h"

namespace algos {

double ProbaTane::CalculateZeroAryFdError(ColumnData const* rhs,
                                          ColumnLayoutRelationData const* /*relation_data*/) {
    return 1 - rhs->GetPositionListIndex()->GetAverageProbability();
}

double ProbaTane::CalculateFdError(model::PositionListIndex const* lhs_pli,
                                   model::PositionListIndex const* joint_pli,
                                   ColumnLayoutRelationData const* /*relation_data*/) {
    return 1 - lhs_pli->GetAverageProbability(*joint_pli);
}

}  // namespace algos
