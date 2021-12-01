#include "api/service/engine_version.h"

#include "json/macro.h"
#include "version.h"

namespace p2pd {

namespace {
struct Result {
    std::string version;
};
} // namespace

namespace json {
DEFINE_MARSHALLER(Result,
    MARSHAL_FIELD(version)
)
} // namespace json

namespace api {
namespace service {

const char * EngineVersion::method() noexcept {
    static const char * method_name = "engine.version";
    return method_name;
}

int EngineVersion::Execute(json::Node const& params, json::Node & result) {
    Result ret;
    ret.version = P2PD_VERSION;
    result << ret;
    return 0;
}

} // namespace service
} // namespace api

} // namespace p2pd