#include "pfdtane.h"

#include <chrono>
#include <iomanip>
#include <list>
#include <memory>

#include <easylogging++.h>

#include "config/error/option.h"
#include "config/error_measure/option.h"
#include "config/error_measure/type.h"
#include "config/max_lhs/option.h"
#include "config/names_and_descriptions.h"
#include "config/option.h"
#include "enums.h"
#include "fd/tane/lattice_level.h"
#include "fd/tane/lattice_vertex.h"
#include "model/table/column_data.h"
#include "model/table/column_layout_relation_data.h"
#include "model/table/relational_schema.h"

namespace algos {
using boost::dynamic_bitset;

PFDTane::PFDTane() : tane::CommonTane() {
    RegisterOptions();
}

double PFDTane::CalculateZeroAryFdError(ColumnData const* rhs) {
    if (error_measure_ == +ErrorMeasure::per_tuple) {
        return PFDTane::CalculateZeroAryFdErrorPerTuple(rhs);
    } else if (error_measure_ == +ErrorMeasure::per_value) {
        return PFDTane::CalculateZeroAryFdErrorPerValue(rhs);
    } else {
        throw config::ConfigurationError("unknown error function");
    }
}

double PFDTane::CalculateFdError(model::PositionListIndex const* x_pli,
                                 model::PositionListIndex const* xa_pli) {
    if (error_measure_ == +ErrorMeasure::per_tuple) {
        return PFDTane::CalculateFdErrorPerTuple(x_pli, xa_pli);
    } else if (error_measure_ == +ErrorMeasure::per_value) {
        return PFDTane::CalculateFdErrorPerValue(x_pli, xa_pli);
    } else {
        throw config::ConfigurationError("unknown error function");
    }
}

double PFDTane::CalculateZeroAryFdErrorPerValue(ColumnData const* rhs) {
    size_t max = 1;
    model::PositionListIndex const* x_index = rhs->GetPositionListIndex();
    for (model::PositionListIndex::Cluster const& x_cluster : x_index->GetIndex()) {
        max = std::max(max, x_cluster.size());
    }
    return 1.0 - static_cast<double>(max) / x_index->getRelationSize();
}

double PFDTane::CalculateFdErrorPerValue(model::PositionListIndex const* x_pli,
                                         model::PositionListIndex const* xa_pli) {
    auto xa_index = xa_pli->GetIndex();
    std::shared_ptr<std::vector<int> const> probing_table = x_pli->CalculateAndGetProbingTable();
    std::sort(xa_index.begin(), xa_index.end(),
              [&probing_table](std::vector<int> const& a, std::vector<int> const& b) {
                  return probing_table->at(a[0]) < probing_table->at(b[0]);
              });
    double sum = 0.0;
    std::size_t cluster_rows_count = 0;
    std::deque<model::PositionListIndex::Cluster> const& x_index = x_pli->GetIndex();
    auto xa_cluster_it = xa_index.begin();

    for (model::PositionListIndex::Cluster const& x_cluster : x_index) {
        std::size_t max = 1;
        for (int x_row : x_cluster) {
            if (xa_cluster_it == xa_index.end()) {
                break;
            }
            if (x_row == xa_cluster_it->at(0)) {
                max = std::max(max, xa_cluster_it->size());
                xa_cluster_it++;
            }
        }
        sum += static_cast<double>(max) / x_cluster.size();
        cluster_rows_count += x_cluster.size();
    }
    auto unique_rows = static_cast<unsigned int>(x_pli->getRelationSize() - cluster_rows_count);
    return 1.0 - (sum + unique_rows) / (x_index.size() + unique_rows);
}

double PFDTane::CalculateZeroAryFdErrorPerTuple(ColumnData const* rhs) {
    size_t max = 1;
    model::PositionListIndex const* x_index = rhs->GetPositionListIndex();
    for (model::PositionListIndex::Cluster const& x_cluster : x_index->GetIndex()) {
        max = std::max(max, x_cluster.size());
    }
    return 1.0 - static_cast<double>(max) / x_index->getRelationSize();
}

double PFDTane::CalculateFdErrorPerTuple(model::PositionListIndex const* x_pli,
                                         model::PositionListIndex const* xa_pli) {
    auto xa_index = xa_pli->GetIndex();
    std::shared_ptr<std::vector<int> const> probing_table = x_pli->CalculateAndGetProbingTable();
    std::sort(xa_index.begin(), xa_index.end(),
              [&probing_table](std::vector<int> const& a, std::vector<int> const& b) {
                  return probing_table->at(a[0]) < probing_table->at(b[0]);
              });

    std::size_t sum = 0;
    std::size_t cluster_rows_count = 0;
    std::deque<model::PositionListIndex::Cluster> const& x_index = x_pli->GetIndex();
    auto xa_cluster_it = xa_index.begin();

    for (model::PositionListIndex::Cluster const& x_cluster : x_index) {
        std::size_t max = 1;
        for (int x_row : x_cluster) {
            if (xa_cluster_it == xa_index.end()) {
                break;
            }
            if (x_row == xa_cluster_it->at(0)) {
                max = std::max(max, xa_cluster_it->size());
                xa_cluster_it++;
            }
        }
        sum += max;
        cluster_rows_count += x_cluster.size();
    }
    auto unique_rows = static_cast<unsigned int>(x_pli->getRelationSize() - cluster_rows_count);
    return 1.0 - static_cast<double>(sum + unique_rows) / x_pli->getRelationSize();
}

void PFDTane::RegisterOptions() {
    RegisterOption(config::ErrorOpt(&max_ucc_error_));
    RegisterOption(config::ErrorMeasureOpt(&error_measure_));
    RegisterOption(config::MaxLhsOpt(&max_lhs_));
}

void PFDTane::MakeExecuteOptsAvailable() {
    MakeOptionsAvailable({config::MaxLhsOpt.GetName(), config::ErrorOpt.GetName(),
                          config::ErrorMeasureOpt.GetName()});
}

model::PositionListIndex* PFDTane::GetColumnIndex(unsigned int column) {
    return relation_->GetColumnData(column).GetPositionListIndex();
}

}  // namespace algos