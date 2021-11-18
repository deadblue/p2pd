#ifndef P2PD_JSON_MACRO_H
#define P2PD_JSON_MACRO_H

#include "json/io.h"

namespace p2pd {
namespace json {

#define UNMARSHAL_FIELD(FIELD) \
    from[#FIELD] >> to.FIELD;

#define UNMARSHAL_VECTOR_FIELD(FIELD)   \
    to.FIELD >>= from[#FIELD];

#define DEFINE_UNMARSHALLER(TYPE, BINDINGS)                 \
    template<>                                              \
    void operator>><TYPE>(Node const& from, TYPE & to) {    \
        BINDINGS                                            \
    }                                                       \
    template<>                                              \
    TYPE& operator>><TYPE>(Node const& from, TYPE && to) {  \
        BINDINGS                                            \
        return to;                                          \
    }

#define MARSHAL_FIELD(FIELD)    \
    to.Attach(#FIELD, Node() << from.FIELD);

#define MARSHAL_VECTOR_FIELD(FIELD) \
    to.Attach(#FIELD, Node() <<= from.FIELD);

#define DEFINE_MARSHALLER(TYPE, BINDINGS)   \
    template<>                                              \
    void operator<<<TYPE>(Node & to, TYPE const& from) {    \
        BINDINGS                                            \
    }                                                       \
    template<>                                              \
    Node& operator<<<TYPE>(Node && to, TYPE const& from) {  \
        BINDINGS                                            \
        return to;                                          \
    }

} // namespace json
} // namespace p2pd

#endif // P2PD_JSON_MACRO_H