#include "api/service/task_get_metadata.h"

#include "engine/type.h"
#include "json/macro.h"

namespace p2pd {

namespace {
struct Params {
    std::string id;
};
} // namespace

namespace json {
DEFINE_UNMARSHALLER(Params,
    UNMARSHAL_FIELD(id)
)
} // namespace json

namespace api {
namespace service {

const char * TaskGetMetadata::method() noexcept {
    return "task.getMetadata";
}

int TaskGetMetadata::Execute(
    json::Node const& params, json::Node & result
) {
    auto p = params >> Params();

    engine::error_code ec;
    engine::TaskMetadata metadata;
    engine_->RetrieveTask(p.id, metadata, ec);

    if(ec) {
        return ec.value();
    } else {
        result << metadata;
        return 0;
    }
}

} // namespace service
} // namespace api
} // namespace p2pd