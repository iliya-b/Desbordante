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
    int max = 1;
    auto const& x_index = rhs->GetPositionListIndex();
    for (auto& x_cluster : x_index->GetIndex()) {
        int total = x_cluster.size();
        if (total > max) max = total;
    }
    return 1.0 - static_cast<double>(max) / x_index->getRelationSize();
}

double ProbaTane::CalculateFdError(model::PositionListIndex const* lhs_pli,
                                   model::PositionListIndex const* joint_pli,
                                   ColumnLayoutRelationData const* /*relation_data*/) {
    double sum = 0.0;
    size_t cluster_rows_count = 0;
    auto& xa_index = joint_pli->GetIndex();
    auto& x_index = lhs_pli->GetIndex();
    auto xa_cluster_iterator = xa_index.begin();

    for (auto& x_cluster : x_index) {
        auto x_row_iterator = x_cluster.begin();
        std::size_t max = 1;
        do {
            if (xa_cluster_iterator != xa_index.end() &&
                *x_row_iterator == xa_cluster_iterator->at(0)) {
                auto cluster_size = xa_cluster_iterator->size();
                if (cluster_size > max) max = cluster_size;
                xa_cluster_iterator++;
            }
        } while (++x_row_iterator != x_cluster.end());
        sum += static_cast<double>(max) / x_cluster.size();
        cluster_rows_count += x_cluster.size();
    }
    unsigned int unique_rows =
            static_cast<unsigned int>(lhs_pli->getRelationSize() - cluster_rows_count);
    return 1.0 - (sum + unique_rows) / (x_index.size() + unique_rows);
}

}  // namespace algos
