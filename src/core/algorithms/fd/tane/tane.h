#pragma once

#include <string>

#include "algorithms/fd/pli_based_fd_algorithm.h"
#include "common_tane.h"
#include "config/error/type.h"
#include "config/max_lhs/type.h"
#include "model/table/position_list_index.h"
#include "model/table/relation_data.h"

namespace algos {

class Tane : public tane::CommonTane {
public:
    double CalculateZeroAryFdError(ColumnData const* rhs) override;
    double CalculateFdError(model::PositionListIndex const* x_pli,
                            model::PositionListIndex const* xa_pli) override;
};

}  // namespace algos
