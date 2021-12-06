#include "api/event.h"
#include "api/message.h"
#include "engine/type.h"
#include "json/macro.h"

namespace p2pd {
namespace json {

// ----- types in api/message.h -----

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

// ----- types in api/event.h -----

DEFINE_MARSHALLER(p2pd::api::event::TaskStateUpdated,
    MARSHAL_FIELD(id)
    MARSHAL_FIELD(old_state)
    MARSHAL_FIELD(new_state)
)

// ----- types in engine/type.h -----

DEFINE_MARSHALLER(p2pd::engine::TaskMetadata::File,
    MARSHAL_FIELD(path)
    MARSHAL_FIELD(size)
)

DEFINE_MARSHALLER(p2pd::engine::TaskMetadata,
    MARSHAL_FIELD(name)
    MARSHAL_FIELD(creator)
    MARSHAL_FIELD(comment)
    MARSHAL_VECTOR_FIELD(files)
    MARSHAL_FIELD(ready)
)

DEFINE_MARSHALLER(p2pd::engine::TaskSummary,
    MARSHAL_FIELD(id)
    MARSHAL_FIELD(name)
    MARSHAL_FIELD_WITH_TYPE(state, int)
    MARSHAL_FIELD(total_bytes)
    MARSHAL_FIELD(dl_bytes)
    MARSHAL_FIELD(dl_speed)
    MARSHAL_FIELD(ul_bytes)
    MARSHAL_FIELD(ul_speed)
    MARSHAL_FIELD(connected_peers)
    MARSHAL_FIELD(connected_seeds)
)

DEFINE_MARSHALLER(p2pd::engine::TaskStatus::Peer,
    MARSHAL_FIELD(client)
    MARSHAL_FIELD(address)
    MARSHAL_FIELD(dl_bytes)
    MARSHAL_FIELD(dl_speed)
    MARSHAL_FIELD(ul_bytes)
    MARSHAL_FIELD(ul_speed)
)

DEFINE_MARSHALLER(p2pd::engine::TaskStatus,
    MARSHAL_VECTOR_FIELD(pieces)
    MARSHAL_VECTOR_FIELD(files)
    MARSHAL_VECTOR_FIELD(peers)
)

} // namespace json
} // namespace p2pd