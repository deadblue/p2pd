#include "engine/type.h"

#include "json/macro.h"

namespace p2pd {
namespace json {

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