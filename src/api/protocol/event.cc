#include "api/protocol/event.h"
#include "json/io.h"

namespace p2pd {
namespace json {

#define FIELD_BINDING(FIELD_NAME)   \
    to.AddChild(#FIELD_NAME, from.FIELD_NAME);

#define DEFINE_MARSHALLER(TYPE_NAME, BINDINGS)                          \
    template<>                                                          \
    void operator<<<TYPE_NAME>(Node & to, TYPE_NAME const& from) {      \
        BINDINGS                                                        \
    }                                                                   \
    template<>                                                          \
    void operator<<<TYPE_NAME>(Node & to, TYPE_NAME && from) {          \
        BINDINGS                                                        \
    }                                                                   \
    template<>                                                          \
    Node& operator<<<TYPE_NAME>(Node && to, TYPE_NAME const& from) {    \
        BINDINGS                                                        \
        return to;                                                      \
    }                                                                   \
    template<>                                                          \
    Node& operator<<<TYPE_NAME>(Node && to, TYPE_NAME && from) {        \
        BINDINGS                                                        \
        return to;                                                      \
    }

DEFINE_MARSHALLER(api::Event,
    FIELD_BINDING(id)
    FIELD_BINDING(name)
    FIELD_BINDING(data)
)

DEFINE_MARSHALLER(api::event::EngineAlert,
    FIELD_BINDING(message)
)

DEFINE_MARSHALLER(api::event::TaskStateChanged,
    FIELD_BINDING(task_id)
    FIELD_BINDING(state)
)

} // namespace json
} // namespace p2pd