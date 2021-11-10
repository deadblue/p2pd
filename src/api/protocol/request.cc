#include "api/protocol/request.h"
#include "json/io.h"

namespace p2pd {
namespace json {

#define FIELD_BINDING(FIELD_NAME) \
    from[#FIELD_NAME] >> to.FIELD_NAME;

// Macro to define unmarshaller for struct
#define DEFINE_UNMARSHALLER(TYPE_NAME, BINDINGS)                \
    template<>                                                  \
    void operator>>(Node const& from, TYPE_NAME & to) {         \
        BINDINGS                                                \
    }                                                           \
    template<>                                                  \
    void operator>>(Node && from, TYPE_NAME & to) {             \
        BINDINGS                                                \
    }                                                           \
    template<>                                                  \
    TYPE_NAME& operator>>(Node const& from, TYPE_NAME && to) {  \
        BINDINGS                                                \
        return to;                                              \
    }                                                           \
    template<>                                                  \
    TYPE_NAME& operator>>(Node && from, TYPE_NAME && to) {      \
        BINDINGS                                                \
        return to;                                              \
    }

DEFINE_UNMARSHALLER(api::Request,
    FIELD_BINDING(id)
    FIELD_BINDING(method)
    FIELD_BINDING(params)
)

DEFINE_UNMARSHALLER(api::request::AddTask,
    FIELD_BINDING(type)
    FIELD_BINDING(uri)
)

} // namespace json
} // namespace p2pd