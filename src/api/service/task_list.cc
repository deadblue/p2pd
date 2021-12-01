#include "api/service/task_list.h"

#include "json/io.h"

namespace p2pd {
namespace api {
namespace service {

const char * TaskList::method() noexcept {
    return "task.list";
}

int TaskList::Execute(
        json::Node const& params, json::Node & result
) {
    // Get task list from engine
    std::vector<engine::TaskSummary> summaries;
    engine_->ListTask(summaries);
    // Convert to json node
    result <<= summaries;
    return 0;
}

} // namespace service
} // namespace api
} // namespace p2pd