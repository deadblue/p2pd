#include "api/event.h"

#include "json/macro.h"

namespace p2pd {
namespace json {

DEFINE_MARSHALLER(api::event::TaskStateUpdated,
    MARSHAL_FIELD(task_id)
    MARSHAL_FIELD(old_state)
    MARSHAL_FIELD(new_state)
)

} // namespace json
} // namespace p2pd