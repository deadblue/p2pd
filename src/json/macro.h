#ifndef P2PD_JSON_MACRO_H
#define P2PD_JSON_MACRO_H

#include "json/io.h"

namespace p2pd {
namespace json {

#define UNMARSHAL_FIELD(FIELD) \
    from[#FIELD] >> to.FIELD;

#define UNMARSHAL_VECTOR_FIELD(FIELD)   \
    to.FIELD >>= from[#FIELD];

#define DEFINE_UNMARSHALLER(STRUCT, BINDINGS)   \
    template<>                                                      \
    void operator>><STRUCT>(Node const& from, STRUCT & to) {        \
        BINDINGS                                                    \
    }                                                               \
    template<>                                                      \
    STRUCT& operator>><STRUCT>(Node const& from, STRUCT && to) {    \
        BINDINGS                                                    \
        return to;                                                  \
    }

#define MARSHAL_FIELD(FIELD)    \
    to.Attach(#FIELD, Node() << from.FIELD);

#define MARSHAL_FIELD_WITH_NAME(FIELD, NAME)    \
    to.Attach(NAME, Node() << from.FIELD);

#define MARSHAL_FIELD_WITH_TYPE(FIELD, TYPE)    \
    to.Attach(#FIELD, Node() << static_cast<TYPE>(from.FIELD));

#define MARSHAL_VECTOR_FIELD(FIELD) \
    to.Attach(#FIELD, Node() <<= from.FIELD);

#define DEFINE_MARSHALLER(STRUCT, BINDINGS) \
    template<>                                                  \
    void operator<<<STRUCT>(Node & to, STRUCT const& from) {    \
        BINDINGS                                                \
    }                                                           \
    template<>                                                  \
    Node& operator<<<STRUCT>(Node && to, STRUCT const& from) {  \
        BINDINGS                                                \
        return to;                                              \
    }

} // namespace json
} // namespace p2pd

#endif // P2PD_JSON_MACRO_H