#include "json/io.h"

namespace p2pd {
namespace json {

#define UNMARSHALLER(TYPE)  \
    template<>                                              \
    void operator>><TYPE>(Node const& from, TYPE & to) {    \
        to = from;                                          \
    }                                                       \
    template<>                                              \
    void operator>><TYPE>(Node && from, TYPE & to) {        \
        to = from;                                          \
    }                                                       \
    template<>                                              \
    TYPE& operator>><TYPE>(Node const& from, TYPE && to) {  \
        to = from; return to;                               \
    }                                                       \
    template<>                                              \
    TYPE& operator>><TYPE>(Node && from, TYPE && to) {      \
        to = from; return to;                               \
    }

UNMARSHALLER(Node)
UNMARSHALLER(bool)
UNMARSHALLER(int)
UNMARSHALLER(int64_t)
UNMARSHALLER(uint64_t)
UNMARSHALLER(double)

template<>
void operator>><std::string>(Node const& from, std::string & to) {
    to = static_cast<const char *>(from);
}
template<>
void operator>><std::string>(Node && from, std::string & to) {
    to = static_cast<const char *>(from);
}
template<>
std::string& operator>><std::string>(Node const& from, std::string && to) {
    to = static_cast<const char *>(from); return to;
}
template<>
std::string& operator>><std::string>(Node && from, std::string && to) {
    to = static_cast<const char *>(from); return to;
}

#define MARSHALLER(TYPE)    \
    template<>                                              \
    void operator<<<TYPE>(Node & to, TYPE const& from) {    \
        to = from;                                          \
    }                                                       \
    template<>                                              \
    void operator<<<TYPE>(Node & to, TYPE && from) {        \
        to = from;                                          \
    }                                                       \
    template<>                                              \
    Node& operator<<<TYPE>(Node && to, TYPE const& from) {  \
        to = from; return to;                               \
    }                                                       \
    template<>                                              \
    Node& operator<<<TYPE>(Node && to, TYPE && from) {      \
        to = from; return to;                               \
    }

MARSHALLER(Node)
MARSHALLER(bool)
MARSHALLER(int)
MARSHALLER(int64_t)
MARSHALLER(uint64_t)
MARSHALLER(double)
MARSHALLER(std::string)

} // namespace json
} // namespace p2pd