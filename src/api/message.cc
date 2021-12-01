#include "api/message.h"

#include "json/macro.h"

namespace p2pd {
namespace json {

DEFINE_UNMARSHALLER(p2pd::api::Request,
    UNMARSHAL_FIELD(id)
    UNMARSHAL_FIELD(method)
    UNMARSHAL_FIELD(params)
)

DEFINE_MARSHALLER(p2pd::api::Response,
    MARSHAL_FIELD(id)
    MARSHAL_FIELD(error)
    MARSHAL_FIELD(result)
)

DEFINE_MARSHALLER(p2pd::api::Event,
    MARSHAL_FIELD(name)
    MARSHAL_FIELD(data)
)

} // namespace json
} // namespace p2pd