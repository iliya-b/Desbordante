#include "pfdtane.h"

#include <chrono>
#include <iomanip>
#include <list>
#include <memory>
#include <queue>

#include <boost/algorithm/string/join.hpp>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <boost/range/join.hpp>
#include <easylogging++.h>

#include "config/error/option.h"
#include "config/error_measure/option.h"
#include "config/max_lhs/option.h"
#include "enums.h"
#include "error/type.h"
#include "fd/tane/lattice_level.h"
#include "fd/tane/lattice_vertex.h"
#include "model/table/column_data.h"
#include "model/table/column_layout_relation_data.h"
#include "model/table/relational_schema.h"
#include "table/column.h"

namespace algos {
using boost::dynamic_bitset;
using Cluster = model::PositionListIndex::Cluster;

PFDTane::PFDTane() : TaneCommon() {
    RegisterOptions();
}

config::ErrorType PFDTane::CalculateZeroAryFdError(ColumnData const* rhs,
                                                   ColumnLayoutRelationData const*) {
    std::size_t max = 1;
    model::PositionListIndex const* x_pli = rhs->GetPositionListIndex();
    for (Cluster const& x_cluster : x_pli->GetIndex()) {
        max = std::max(max, x_cluster.size());
    }
    return 1.0 - static_cast<double>(max) / x_pli->GetRelationSize();
}

config::ErrorType PFDTane::CalculateFdError(model::PositionListIndex const* x_pli,
                                            model::PositionListIndex const* xa_pli,
                                            ErrorMeasure measure, ColumnLayoutRelationData const*) {
    std::deque<Cluster> xa_index = xa_pli->GetIndex();
    std::shared_ptr<Cluster const> probing_table = x_pli->CalculateAndGetProbingTable();
    std::sort(xa_index.begin(), xa_index.end(),
              [&probing_table](Cluster const& a, Cluster const& b) {
                  return probing_table->at(a.front()) < probing_table->at(b.front());
              });
    double sum = 0.0;
    std::size_t cluster_rows_count = 0;
    std::deque<Cluster> const& x_index = x_pli->GetIndex();
    auto xa_cluster_it = xa_index.begin();

    for (Cluster const& x_cluster : x_index) {
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
        sum += measure == +ErrorMeasure::per_tuple ? static_cast<double>(max)
                                                   : static_cast<double>(max) / x_cluster.size();
        cluster_rows_count += x_cluster.size();
    }
    unsigned int unique_rows =
            static_cast<unsigned int>(x_pli->GetRelationSize() - cluster_rows_count);
    double probability = static_cast<double>(sum + unique_rows) /
                         (measure == +ErrorMeasure::per_tuple ? x_pli->GetRelationSize()
                                                              : x_index.size() + unique_rows);
    return 1.0 - probability;
}

config::ErrorType PFDTane::CalculateZeroAryFdError(ColumnData const* rhs) {
    return CalculateZeroAryFdError(rhs, relation_.get());
}

config::ErrorType PFDTane::CalculateFdError(model::PositionListIndex const* lhs_pli,
                                            model::PositionListIndex const* joint_pli) {
    return CalculateFdError(lhs_pli, joint_pli, error_measure_, relation_.get());
}

void PFDTane::RegisterOptions() {
    RegisterOption(config::kErrorOpt(&max_ucc_error_));
    RegisterOption(config::kErrorMeasureOpt(&error_measure_));
    RegisterOption(config::kMaxLhsOpt(&max_lhs_));
}

void PFDTane::MakeExecuteOptsAvailable() {
    MakeOptionsAvailable({config::kErrorOpt.GetName(), config::kErrorMeasureOpt.GetName(),
                          config::kMaxLhsOpt.GetName()});
}

// recalculate probability of FDs (with one-attribute LHS)
void PFDTane::Recalculate() {
    config::ErrorType t1 = 0.0;
    config::ErrorType t2 = 0.0;
    // meanwhile t1 < 1 - max_fd_error_ < t2
    // we keep those FDs with probability > t2
    // we remove those FDs with probability < t1
    // we recalculate probability of the rest
    // and keep those with probability >= 1 - max_fd_error_

    std::list<FD> list = FdList(); // todo: remove non 1-LHS and add inferrable 1-LHS
    unsigned long fds_count = static_cast<unsigned long>(list.size());
    unsigned long subsets_count = std::pow(2L, fds_count - 1);
    for(FD& fd : list){
    
        config::ErrorType probability = 1 - 0.0; // TODO: use actual pFD probaility
        if(probability < t1 || probability > t2) continue;
        if(fd.GetLhsIndices().size() > 1) continue; // we support FDs with one-attribute LHS
        if(fd.GetLhsIndices().size() == 0) {
            // todo: spread them before the loop
        }

        // enumerate all subsets of FDs
        auto subset = boost::dynamic_bitset<>(list.size());
        for (std::size_t i = 0; i < subsets_count; i++){
            // todo: prepare subset

            // let current fd be: A->B
            // let subset be a set of FDs represented by a directed clustered graph
            // 1. Calculate P(subset)
            // 1.1. Check transitivity of the whole set  
            // 1.1.1. Calculate the transitive closure of the graph
            // 1.1.2. Check if the transitive clousure is a subgraph of the original graph
            // 1.2. Calculate P(subset) as product of computed probabilities for other FDs
            // 2. Calculate P(fd|subset)
            // 2.1. Check existence of C s.t. there are A -> C and C -> B
            // 2.2. Check existence of B -> C and non-existence of A -> C
            // 2.3. Check existence of C -> A and non-existence of C -> B
            // 2.4. Calculate using Bayes rule
            // 2.4.1. Find all C s.t. there are both A->C and B->C 
            // 2.4.2. Find all C s.t. there are both C->A and C->B 
            // 2.4.3. Calculate using apriori probabilities
        }

        // iterate unless converges


    }
}

}  // namespace algos
