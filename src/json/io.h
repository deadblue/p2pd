#ifndef P2PD_JSON_IO_H
#define P2PD_JSON_IO_H

#include <string>

#include "json/node.h"

namespace p2pd {
namespace json {

// ----- Unmarshal operators -----
template<typename T>
void operator>>(Node const& from, T & to);
template<typename T>
void operator>>(Node && from, T & to);
template<typename T>
T& operator>>(Node const& from, T && to);
template<typename T>
T& operator>>(Node && from, T && to);
// ----- Marshal operators -----
template<typename T>
void operator<<(Node & to, T const& from);
template<typename T>
void operator<<(Node & to, T && from);
template<typename T>
Node& operator<<(Node && to, T const& from);
template<typename T>
Node& operator<<(Node && to, T && from);

#define DECLARE_CONVERTOR(VALUE_TYPE)                           \
    void operator>>(Node const& from, VALUE_TYPE & to);         \
    void operator>>(Node && from, VALUE_TYPE & to);             \
    VALUE_TYPE& operator>>(Node const& from, VALUE_TYPE && to); \
    VALUE_TYPE& operator>>(Node && from, VALUE_TYPE && to);     \
    void operator<<(Node & to, VALUE_TYPE const& from);         \
    void operator<<(Node & to, VALUE_TYPE && from);             \
    Node& operator<<(Node && to, VALUE_TYPE const& from);       \
    Node& operator<<(Node && to, VALUE_TYPE && from);

DECLARE_CONVERTOR(Node)
DECLARE_CONVERTOR(bool)
DECLARE_CONVERTOR(int)
DECLARE_CONVERTOR(int64_t)
DECLARE_CONVERTOR(uint64_t)
DECLARE_CONVERTOR(double)
DECLARE_CONVERTOR(std::string)

template<typename T>
void ParseAs(const char * str, T & ret) {
    Parse(str) >> ret;
}
template<typename T>
void ParseAs(std::string const& str, T & ret) {
    Parse(str) >> ret;
}

template<typename T>
std::string ToString(T const& value) {
    return (Node() << value).ToString();
}
template<typename T>
std::string ToString(T && value) {
    return (Node() << std::move(value)).ToString();
}

} // namespace json
} // namespace p2pd

#endif // P2PD_JSON_IO_H