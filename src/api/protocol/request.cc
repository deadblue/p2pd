#include "api/protocol/request.h"

#include "api/protocol/macro.h"
#include "json/io.h"

namespace p2pd {
namespace json {

DEFINE_UNMARSHALLER(api::Request,
    UNMARSHAL_FIELD(id)
    UNMARSHAL_FIELD(method)
    UNMARSHAL_FIELD(params)
)

DEFINE_UNMARSHALLER(api::request::AddTask,
    UNMARSHAL_FIELD(type)
    UNMARSHAL_FIELD(uri)
)

} // namespace json
} // namespace p2pd