#include "api/request.h"
#include "json/io.h"

namespace p2pd {
namespace json {

#define FIELD_BINDING(FIELD_NAME) \
    from[#FIELD_NAME] >> to.FIELD_NAME;

// Macro to define unmarshaller for struct
#define DEFINE_UNMARSHALLER(TYPE_NAME, BINDINGS)        \
    template<>                                          \
    void operator>>(Node const& from, TYPE_NAME & to) { \
        BINDINGS                                        \
    }                                                   \
    template<>                                          \
    void operator>>(Node && from, TYPE_NAME & to) {     \
        BINDINGS                                        \
    }

DEFINE_UNMARSHALLER(api::Request,
    FIELD_BINDING(id)
    FIELD_BINDING(method)
    FIELD_BINDING(params)
)

} // namespace json
} // namespace p2pd