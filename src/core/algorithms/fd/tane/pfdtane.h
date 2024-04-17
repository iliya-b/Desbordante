#pragma once

#include "algorithms/fd/pli_based_fd_algorithm.h"
#include "config/error/type.h"
#include "enums.h"
#include "model/table/column_data.h"
#include "model/table/column_layout_relation_data.h"
#include "model/table/position_list_index.h"
#include "tane_common.h"

namespace algos {

class PFDTane : public tane::TaneCommon {
private:
    ErrorMeasure error_measure_ = +ErrorMeasure::per_tuple;
    void RegisterOptions();
    void MakeExecuteOptsAvailableFDInternal() final;
    config::ErrorType CalculateZeroAryFdError(ColumnData const* rhs) override;
    config::ErrorType CalculateFdError(model::PositionListIndex const* lhs_pli,
                                       model::PositionListIndex const* joint_pli) override;

public:
    PFDTane(std::optional<ColumnLayoutRelationDataManager> relation_manager = std::nullopt);
    static config::ErrorType CalculateZeroAryPFDError(
            ColumnData const* rhs, ColumnLayoutRelationData const* relation_data);
    static config::ErrorType CalculatePFDError(model::PositionListIndex const* x_pli,
                                               model::PositionListIndex const* xa_pli,
                                               ErrorMeasure error_measure,
                                               ColumnLayoutRelationData const* relation_data);
};

}  // namespace algos
