#pragma once

#include "config/error/type.h"
#include "model/table/position_list_index.h"
#include "tane_common.h"

namespace algos {

class Tane : public TaneCommon {
private:
    void RegisterOptions();
    void MakeExecuteOptsAvailable() final;
    config::ErrorType CalculateZeroAryFdError(ColumnData const* rhs) override;
    config::ErrorType CalculateFdError(model::PositionListIndex const* lhs_pli,
                                       model::PositionListIndex const* joint_pli) override;

public:
    Tane();
    static config::ErrorType CalculateZeroAryAFDError(
            ColumnData const* rhs, ColumnLayoutRelationData const* relation_data);
    static config::ErrorType CalculateAFDError(model::PositionListIndex const* lhs_pli,
                                               model::PositionListIndex const* joint_pli,
                                               ColumnLayoutRelationData const* relation_data);
};

}  // namespace algos
