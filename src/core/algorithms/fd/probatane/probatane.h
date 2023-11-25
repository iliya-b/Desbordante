#pragma once

#include <string>

#include "algorithms/fd/tane/tane.h"
#include "config/error/type.h"
#include "config/max_lhs/type.h"
#include "model/table/position_list_index.h"
#include "model/table/relation_data.h"

namespace algos {

class ProbaTane : public Tane {
public:
    double CalculateZeroAryFdError(ColumnData const* rhs, ColumnLayoutRelationData const*) override;
    double CalculateFdError(model::PositionListIndex const* lhs_pli,
                            model::PositionListIndex const* joint_pli,
                            ColumnLayoutRelationData const*) override;
};

}  // namespace algos