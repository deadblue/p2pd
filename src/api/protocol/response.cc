#include "api/protocol/response.h"
#include "json/io.h"

namespace p2pd {
namespace json {

#define FIELD_BINDING(FIELD_NAME)   \
    to.AddChild(#FIELD_NAME, from.FIELD_NAME);

// Macro to define unmarshaller from struct
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

DEFINE_MARSHALLER(api::Response,
    FIELD_BINDING(id)
    FIELD_BINDING(error)
    FIELD_BINDING(result)
)

} // namespace json
} // namespace p2pd