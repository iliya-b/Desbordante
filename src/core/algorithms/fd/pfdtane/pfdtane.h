#pragma once

#include <string>

#include "algorithms/fd/tane/tane.h"
#include "config/error_measure/type.h"
#include "enums.h"
#include "model/table/position_list_index.h"
#include "model/table/relation_data.h"

namespace algos {

class PFDTane : public PliBasedFDAlgorithm {
private:
    void ResetStateFd() final;
    void RegisterOptions();
    void MakeExecuteOptsAvailable() final;
    unsigned long long ExecuteInternal() override final;

public:
    config::ErrorType max_fd_error_;
    config::ErrorType max_ucc_error_;
    config::MaxLhsType max_lhs_;
    ErrorMeasure error_measure_ = ErrorMeasure::_values()[0];

    int count_of_fd_ = 0;
    int count_of_ucc_ = 0;
    long apriori_millis_ = 0;

    PFDTane();

    model::PositionListIndex* GetColumnIndex(unsigned int column);  // for test

    static double CalculateUccError(model::PositionListIndex const* pli,
                                    ColumnLayoutRelationData const* relation_data);

    // static double round(double error) { return ((int)(error * 32768) + 1)/ 32768.0; }

    void RegisterAndCountFd(Vertical const& lhs, Column const* rhs, double error,
                            RelationalSchema const* schema);
    // void RegisterFd(Vertical const* lhs, Column const* rhs, double error, RelationalSchema const*
    // schema);
    void RegisterUcc(Vertical const& key, double error, RelationalSchema const* schema);

    double CalculateZeroAryFdErrorPerValue(ColumnData const* rhs);
    double CalculateFdErrorPerValue(model::PositionListIndex const* x_pli,
                                    model::PositionListIndex const* xa_pli);
    double CalculateZeroAryFdErrorPerTuple(ColumnData const* rhs);
    double CalculateFdErrorPerTuple(model::PositionListIndex const* x_pli,
                                    model::PositionListIndex const* xa_pli);
    double CalculateZeroAryG1(ColumnData const* rhs);
    double CalculateG1(model::PositionListIndex const* x_pli,
                       model::PositionListIndex const* xa_pli);
};

}  // namespace algos
