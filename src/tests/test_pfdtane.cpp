#include <algorithm>
#include <filesystem>
#include <memory>

#include <gtest/gtest.h>

#include "algo_factory.h"
#include "builtin.h"
#include "config/indices/type.h"
#include "datasets.h"
#include "fd/pfdtane/pfdtane.h"
#include "fd/pfdtane/pfdtane.h"

namespace tests {
namespace onam = config::names;

struct PFDTaneParams {
    algos::StdParamsMap params;
    long double const error = 0.;
    size_t const num_error_clusters = 0;
    size_t const num_error_rows = 0;

    PFDTaneParams(config::IndicesType lhs_indices, config::IndicesType rhs_indices,
                      size_t const num_error_clusters = 0, size_t const num_error_rows = 0,
                      long double const error = 0., char const* dataset = "TestFD.csv",
                      char const separator = ',', bool const has_header = true)
        : params({{onam::kLhsIndices, std::move(lhs_indices)},
                  {onam::kRhsIndices, std::move(rhs_indices)},
                  {onam::kCsvPath, test_data_dir / dataset},
                  {onam::kSeparator, separator},
                  {onam::kHasHeader, has_header},
                  {onam::kEqualNulls, true}}),
          error(error),
          num_error_clusters(num_error_clusters),
          num_error_rows(num_error_rows) {}
};

class TestPFDTane : public ::testing::TestWithParam<PFDTaneParams> {};

TEST_P(TestPFDTane, DefaultTest) {
    auto const& p = GetParam();
    auto mp = algos::StdParamsMap(p.params);
    auto algos = algos::CreateAndLoadAlgorithm<algos::PFDTane>(mp);
    algos->Execute();
 
    

}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
        PFDTaneTestSuite, TestPFDTane,
        ::testing::Values(
            PFDTaneParams({1}, {0}),
            PFDTaneParams({2}, {0}),
            PFDTaneParams({2}, {1}),
            PFDTaneParams({0, 1, 2, 3, 4}, {5}),
            PFDTaneParams({2, 3}, {5}),
            PFDTaneParams({5}, {0}),
            PFDTaneParams({5}, {1}),
            PFDTaneParams({5}, {2}),
            PFDTaneParams({5}, {3}),
            PFDTaneParams({5}, {4}),
            PFDTaneParams({1, 3}, {4}),
            PFDTaneParams({5}, {0, 1, 2, 3, 4}),
            PFDTaneParams({2}, {0, 1}),
            PFDTaneParams({2, 3}, {0, 1, 4, 5}),
            PFDTaneParams({2, 4}, {0, 1, 3, 5}),
            PFDTaneParams({3, 4}, {0, 1}),
            PFDTaneParams({1, 4}, {0, 3}),
            PFDTaneParams({1, 3}, {0, 3}),
            PFDTaneParams({4}, {3}, 1, 2, 2.L/132),
            PFDTaneParams({3}, {4}, 2, 10, 26.L/132),
            PFDTaneParams({0}, {1}, 1, 12, 108.L/132),
            PFDTaneParams({1}, {2}, 4, 12, 16.L/132),
            PFDTaneParams({1}, {3}, 2, 6, 8.L/132),
            PFDTaneParams({1}, {2, 3}, 4, 12, 18.L/132),
            PFDTaneParams({2}, {5}, 1, 2, 2.L/132),
            PFDTaneParams({1, 3}, {5}, 3, 8, 10.L/132),
            PFDTaneParams({1, 2}, {0, 3}, 1, 2, 2.L/132),
            PFDTaneParams({3, 4}, {1, 2}, 3, 8, 10.L/132),
            PFDTaneParams({2}, {3, 4}, 1, 2, 2.L/132),
            PFDTaneParams({4}, {1, 2}, 4, 10, 12.L/132),
            PFDTaneParams({0}, {2, 3}, 1, 12, 126.L/132),
            PFDTaneParams({1, 4}, {2, 3, 5}, 3, 8, 10.L/132),
            PFDTaneParams({0, 1}, {1, 4}, 2, 6, 8.L/132)
            ));
// clang-format on

}  // namespace tests
