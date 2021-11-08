#include "json/io.h"

namespace p2pd {
namespace json {

#define DEFINE_CONVERTOR(VALUE_TYPE)                                \
    void operator>>(Node const& from, VALUE_TYPE & to) {            \
        to = from;                                                  \
    }                                                               \
    void operator>>(Node && from, VALUE_TYPE & to) {                \
        to = from;                                                  \
    }                                                               \
    VALUE_TYPE& operator>>(Node const& from, VALUE_TYPE && to) {    \
        to = from;                                                  \
        return to;                                                  \
    }                                                               \
    VALUE_TYPE& operator>>(Node && from, VALUE_TYPE && to) {        \
        to = from;                                                  \
        return to;                                                  \
    }                                                               \
    void operator<<(Node & to, VALUE_TYPE const& from) {            \
        to = from;                                                  \
    }                                                               \
    void operator<<(Node & to, VALUE_TYPE && from) {                \
        to = from;                                                  \
    }                                                               \
    Node& operator<<(Node && to, VALUE_TYPE const& from) {          \
        to = from;                                                  \
        return to;                                                  \
    }                                                               \
    Node& operator<<(Node && to, VALUE_TYPE && from) {              \
        to = from;                                                  \
        return to;                                                  \
    }

DEFINE_CONVERTOR(Node)
DEFINE_CONVERTOR(bool)
DEFINE_CONVERTOR(int)
DEFINE_CONVERTOR(int64_t)
DEFINE_CONVERTOR(uint64_t)
DEFINE_CONVERTOR(double)

void operator>>(Node const& from, std::string & to) {
    to = static_cast<const char *>(from);
}
void operator>>(Node && from, std::string & to) {
    to = static_cast<const char *>(from);
}
std::string& operator>>(Node const& from, std::string && to) {
    to = static_cast<const char *>(from);
    return to;
}
std::string& operator>>(Node && from, std::string && to) {
    to = static_cast<const char *>(from);
    return to;
}
void operator<<(Node & to, std::string const& from) {
    to = from.c_str();
}
void operator<<(Node & to, std::string && from) {
    to = from.c_str();
}
Node& operator<<(Node && to, std::string const& from) {
    to = from.c_str();
    return to;
}
Node& operator<<(Node && to, std::string && from) {
    to = from.c_str();
    return to;
}

} // namespace json
} // namespace p2pd