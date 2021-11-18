#include "api/protocol/event.h"

#include "json/macro.h"

namespace p2pd {
namespace json {

DEFINE_MARSHALLER(api::Event,
    MARSHAL_FIELD(id)
    MARSHAL_FIELD(name)
    MARSHAL_FIELD(data)
)

DEFINE_MARSHALLER(api::event::EngineAlert,
    MARSHAL_FIELD(message)
)

DEFINE_MARSHALLER(api::event::TaskStateChanged,
    MARSHAL_FIELD(task_id)
    MARSHAL_FIELD(state)
)

} // namespace json
} // namespace p2pd