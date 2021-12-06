#include "api/service/task_get_status.h"

#include "engine/error_code.h"
#include "engine/type.h"
#include "json/macro.h"

namespace p2pd {

namespace {
struct TaskInspectParams {
    std::string id;
};
} // namespace

namespace json {
DEFINE_UNMARSHALLER(TaskInspectParams,
    UNMARSHAL_FIELD(id)
)
} // namespace json

namespace api {
namespace service {

const char * TaskGetStatus::method() noexcept {
    return "task.getStatus";
}

int TaskGetStatus::Execute(
    json::Node const& params, json::Node & result
) {
    auto p = params >> TaskInspectParams();

    engine::error_code ec;
    engine::TaskStatus status;
    engine_->InspectTask(p.id, status, ec);
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