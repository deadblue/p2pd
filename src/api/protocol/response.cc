#include "api/protocol/response.h"

#include "api/protocol/macro.h"
#include "json/io.h"

namespace p2pd {
namespace json {

DEFINE_MARSHALLER(api::Response,
    MARSHAL_FIELD(id)
    MARSHAL_FIELD(error)
    MARSHAL_FIELD(result)
)

} // namespace json
} // namespace p2pd