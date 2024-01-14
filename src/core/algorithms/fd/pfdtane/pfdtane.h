#pragma once

#include <string>

#include "algorithms/fd/tane/common_tane.h"
#include "config/error_measure/type.h"
#include "enums.h"
#include "model/table/position_list_index.h"
#include "model/table/relation_data.h"

namespace algos {

class PFDTane : public tane::CommonTane {
private:
    void RegisterOptions();
    void MakeExecuteOptsAvailable() final;

public:
    ErrorMeasure error_measure_ = +ErrorMeasure::per_value;

    PFDTane();

    model::PositionListIndex* GetColumnIndex(unsigned int column);  // for test

    double CalculateZeroAryFdError(ColumnData const* rhs) override;
    double CalculateFdError(model::PositionListIndex const* x_pli,
                            model::PositionListIndex const* xa_pli) override;
    double CalculateZeroAryFdErrorPerValue(ColumnData const* rhs);
    double CalculateFdErrorPerValue(model::PositionListIndex const* x_pli,
                                    model::PositionListIndex const* xa_pli);
    double CalculateZeroAryFdErrorPerTuple(ColumnData const* rhs);
    double CalculateFdErrorPerTuple(model::PositionListIndex const* x_pli,
                                    model::PositionListIndex const* xa_pli);
};

}  // namespace algos