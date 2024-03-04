#include <algorithm>
#include <filesystem>

#include <gtest/gtest.h>

#include "algo_factory.h"
#include "all_tables_config.h"
#include "config/names.h"
#include "fd/pfdtane/enums.h"
#include "fd/pfdtane/pfdtane.h"
#include "model/table/column_layout_relation_data.h"
#include "table_config.h"

namespace tests {
namespace onam = config::names;
using boost::dynamic_bitset;

struct pFD {
    std::size_t lhs_id;
    std::size_t rhs_id;
    config::ErrorType expected_error;
};

struct PFDTaneMiningParams {
    algos::StdParamsMap params;
    unsigned int result_hash;

    PFDTaneMiningParams(unsigned int result_hash, config::ErrorType const error,
                        algos::ErrorMeasure error_measure, TableConfig table_config)
        : params({{onam::kCsvPath, table_config.GetPath()},
                  {onam::kSeparator, table_config.separator},
                  {onam::kHasHeader, table_config.has_header},
                  {onam::kError, error},
                  {onam::kErrorMeasure, error_measure},
                  {onam::kEqualNulls, true}}),
          result_hash(result_hash) {}
};

struct PFDTaneValidationParams {
    TableConfig table_config;
    algos::ErrorMeasure error_measure;
    std::vector<pFD> fds;

    PFDTaneValidationParams(std::vector<pFD> fds, algos::ErrorMeasure error_measure,
                            TableConfig table_config)
        : table_config(table_config), error_measure(error_measure), fds(fds) {}
};

class TestPFDTaneMining : public ::testing::TestWithParam<PFDTaneMiningParams> {};

class TestPFDTaneValidation : public ::testing::TestWithParam<PFDTaneValidationParams> {};

TEST_P(TestPFDTaneMining, DefaultTest) {
    auto const &p = GetParam();
    auto mp = algos::StdParamsMap(p.params);
    auto algos = algos::CreateAndLoadAlgorithm<algos::PFDTane>(mp);
    algos->Execute();
    EXPECT_EQ(p.result_hash, algos->Fletcher16());
}

TEST_P(TestPFDTaneValidation, ErrorCalculationTest) {
    auto const &p = GetParam();
    double eps = 0.0001;
    auto table = std::make_shared<CSVParser>(p.table_config.GetPath(), p.table_config.separator,
                                             p.table_config.has_header);
    auto relation = ColumnLayoutRelationData::CreateFrom(*table, true);
    for (std::size_t i = 0; i < p.fds.size(); i++) {
        pFD fd = p.fds[i];
        auto lhs = relation->GetColumnData(fd.lhs_id).GetPositionListIndex();
        auto rhs = relation->GetColumnData(fd.rhs_id).GetPositionListIndex();

        config::ErrorType error =
                algos::PFDTane::CalculateFdError(lhs, lhs->Intersect(rhs).get(), p.error_measure);
        EXPECT_NEAR(error, fd.expected_error, eps);
    }
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
        PFDTaneTestMiningSuite, TestPFDTaneMining,
        ::testing::Values(
            PFDTaneMiningParams(44381, 0.3, +algos::ErrorMeasure::per_value, kTestFD)
        ));


INSTANTIATE_TEST_SUITE_P(
        PFDTaneTestValidationSuite, TestPFDTaneValidation,
        ::testing::Values(
                    PFDTaneValidationParams({{2, 3, 0.0625}, {4, 5, 0.333333}, {3, 2, 0.291666}, {0, 1, 0.75},
                                             {1, 0, 0.0}, {4, 3, 0.099999}, {1, 5, 0.416666}, {5, 1, 0.0}}, +algos::ErrorMeasure::per_value, kTestFD),
                
                    PFDTaneValidationParams({{2, 3, 0.083333}, {4, 5, 0.333333}, {3, 2, 0.5}, {0, 1, 0.75},
                                             {1, 0, 0.0}, {4, 3, 0.083333}, {1, 5, 0.416666}, {5, 1, 0.0}}, +algos::ErrorMeasure::per_tuple, kTestFD)
                
                ));

// clang-format on

}  // namespace tests
