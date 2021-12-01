#include "api/service/task_inspect.h"

#include "engine/error_code.h"
#include "engine/type.h"
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

const char * TaskInspect::method() noexcept {
    return "task.inspect";
}

int TaskInspect::Execute(
    json::Node const& params, json::Node & result
) {
    auto p = params >> TaskInspectParams();

    engine::error_code ec;
    engine::TaskStatus status;
    engine_->InspectTask(p.task_id, status, ec);
    if(ec) {
        return ec.value();
    } else {
        result << status;
        return 0;
    }
}

} // namespace service
} // namespace api
} // namespace p2pd