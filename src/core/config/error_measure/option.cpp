#include "config/error_measure/option.h"

#include "config/exceptions.h"
#include "config/names_and_descriptions.h"

namespace config {
using names::kErrorMeasure, descriptions::kDErrorMeasure;
extern CommonOption<ErrorMeasureType> const ErrorMeasureOpt{
        kErrorMeasure, kDErrorMeasure, "e", {}, [](ErrorMeasureType error) {
            if (!(error == "e" || error == "per_value" || error == "per_tuple")) {
                throw ConfigurationError("ERROR: error function can be e, per_value or per_tuple");
            }
        }};
}  // namespace config