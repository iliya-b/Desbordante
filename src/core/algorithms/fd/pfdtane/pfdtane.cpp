#include "pfdtane.h"

namespace algos {

double PFDTane::CalculateZeroAryFdError(ColumnData const* rhs) {
    size_t max = 1;
    model::PositionListIndex const* x_index = rhs->GetPositionListIndex();
    for (model::PositionListIndex::Cluster const& x_cluster : x_index->GetIndex()) {
        max = std::max(max, x_cluster.size());
    }
    return 1.0 - static_cast<double>(max) / x_index->getRelationSize();
}

double PFDTane::CalculateFdError(model::PositionListIndex const* x_pli,
                                 model::PositionListIndex const* xa_pli) {
    double sum = 0.0;
    std::size_t cluster_rows_count = 0;
    std::deque<model::PositionListIndex::Cluster> const& xa_index = xa_pli->GetIndex();
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

}  // namespace algos
