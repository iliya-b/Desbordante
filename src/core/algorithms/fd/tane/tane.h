#pragma once

#include "algorithms/fd/pli_based_fd_algorithm.h"
#include "algorithms/fd/tane/model/lattice_level.h"
#include "config/error/type.h"
#include "enums.h"
#include "model/table/column_data.h"
#include "model/table/column_layout_relation_data.h"
#include "model/table/position_list_index.h"

namespace algos {

class Tane : public tane::TaneCommon {
private:
    void MakeExecuteOptsAvailableFDInternal() override final;
    config::ErrorType CalculateZeroAryFdError(ColumnData const* rhs) override;
    config::ErrorType CalculateFdError(model::PositionListIndex const* lhs_pli,
                                       model::PositionListIndex const* joint_pli) override;

public:
    Tane(std::optional<ColumnLayoutRelationDataManager> relation_manager = std::nullopt);
};

}  // namespace algos
