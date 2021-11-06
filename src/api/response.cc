#include "api/response.h"
#include "json/binding.h"

namespace p2pd {
namespace json {

#define FIELD_BINDING(FIELD_NAME)   \
    to.AddChild(#FIELD_NAME, from.FIELD_NAME);

// Macro to define unmarshaller from struct
#define DEFINE_MARSHALLER(TYPE_NAME, BINDINGS)              \
    template<>                                              \
    Node& operator<<(Node && to, TYPE_NAME const& from) {   \
        BINDINGS                                            \
        return to;                                          \
    }                                                       \
    template<>                                              \
    Node& operator<<(Node && to, TYPE_NAME && from) {       \
        BINDINGS                                            \
        return to;                                          \
    }

DEFINE_MARSHALLER(api::GenericResponse,
    FIELD_BINDING(id)
    FIELD_BINDING(action)
    FIELD_BINDING(error)
    FIELD_BINDING(result)
)

} // namespace json
} // namespace p2pd