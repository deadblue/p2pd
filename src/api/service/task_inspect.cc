#include "api/service/task_inspect.h"

#include "json/macro.h"

namespace p2pd {

namespace {
struct TaskInspectParams {
    std::string task_id;
};
} // namespace

namespace json {
DEFINE_UNMARSHALLER(TaskInspectParams,
    UNMARSHAL_FIELD(task_id)
)
} // namespace json

namespace api {
namespace service {

int TaskInspect::Execute(
    json::Node const& params, json::Node & result
) {
    auto p = params >> TaskInspectParams();
    return 0;
}

} // namespace service
} // namespace api
} // namespace p2pd