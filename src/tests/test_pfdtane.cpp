#include <algorithm>
#include <filesystem>

#include <gtest/gtest.h>

#include "algo_factory.h"
#include "config/names.h"
#include "fd/pfdtane/pfdtane.h"
#include "table_config.h"

namespace tests {
namespace onam = config::names;

struct PFDTaneParams {
    algos::StdParamsMap params;
    long double const error = 0.;
    std::string result;

    PFDTaneParams(std::string result, double const error = 0., char const *dataset = "TestFD.csv",
                  char const separator = ',', bool const has_header = true)
        : params({{onam::kCsvPath, test_data_dir / dataset},
                  {onam::kSeparator, separator},
                  {onam::kHasHeader, has_header},
                  {onam::kError, error},
                  {onam::kEqualNulls, true}}),
          error(error),
          result(result) {}
};

class TestPFDTane : public ::testing::TestWithParam<PFDTaneParams> {};

TEST_P(TestPFDTane, DefaultTest) {
    auto const &p = GetParam();
    auto mp = algos::StdParamsMap(p.params);
    auto algos = algos::CreateAndLoadAlgorithm<algos::PFDTane>(mp);
    algos->Execute();
    EXPECT_EQ(p.result, algos->GetJsonFDs());
}

TEST_P(TestPFDTane, PerTupleTest) {
    auto const &p = GetParam();
    auto mp = algos::StdParamsMap(p.params);
    auto algos = algos::CreateAndLoadAlgorithm<algos::PFDTane>(mp);

    std::vector<std::pair<int, int>> cols = {{2, 3}, {4, 5}, {3, 2}, {0, 1},
                                             {1, 0}, {4, 3}, {1, 5}, {5, 1}};
    std::vector<double> errors = {0.083333, 0.333333, 0.5, 0.75, 0.0, 0.083333, 0.416666, 0.0};
    double eps = 0.0001;

    for (std::size_t i = 0; i < cols.size(); i++) {
        double error = algos->CalculateFdErrorPerTuple(
                algos->GetColumnIndex(cols[i].first),
                algos->GetColumnIndex(cols[i].first)
                        ->Intersect(algos->GetColumnIndex(cols[i].second))
                        .get());
        EXPECT_NEAR(error, errors[i], eps);
    }
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
        PFDTaneTestSuite, TestPFDTane,
        ::testing::Values(
            PFDTaneParams(R"({"fds": [{"lhs": [1,4], "rhs": 2},{"lhs": [1,4], "rhs": 5},{"lhs": [1], "rhs": 3},{"lhs": [1], "rhs": 4},{"lhs": [2], "rhs": 1},{"lhs": [2], "rhs": 3},{"lhs": [2], "rhs": 4},{"lhs": [2], "rhs": 5},{"lhs": [3], "rhs": 1},{"lhs": [3], "rhs": 2},{"lhs": [3], "rhs": 4},{"lhs": [3], "rhs": 5},{"lhs": [4], "rhs": 1},{"lhs": [4], "rhs": 3},{"lhs": [5], "rhs": 1},{"lhs": [5], "rhs": 2},{"lhs": [5], "rhs": 3},{"lhs": [5], "rhs": 4},{"lhs": [], "rhs": 0}]})", 0.3)
        ));

// clang-format on

}  // namespace tests
