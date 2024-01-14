#include "config/error_measure/option.h"

#include "config/exceptions.h"
#include "config/names_and_descriptions.h"
#include "fd/pfdtane/enums.h"

namespace config {
using names::kErrorMeasure, descriptions::kDErrorMeasure;
extern CommonOption<ErrorMeasureType> const ErrorMeasureOpt{
        kErrorMeasure,
        kDErrorMeasure,
        algos::ErrorMeasure::_values()[0],
        {},
        [](ErrorMeasureType error) {
            if (!(error == +algos::ErrorMeasure::per_value ||
                  error == +algos::ErrorMeasure::per_tuple)) {
                throw ConfigurationError("ERROR: error function can be per_value or per_tuple");
            }
        }};
}  // namespace config