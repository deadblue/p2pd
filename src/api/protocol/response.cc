#include "api/protocol/response.h"

#include "json/macro.h"

namespace p2pd {
namespace json {

DEFINE_MARSHALLER(api::Response,
    MARSHAL_FIELD(id)
    MARSHAL_FIELD(error)
    MARSHAL_FIELD(result)
)

} // namespace json
} // namespace p2pd