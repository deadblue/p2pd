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

// ----- Shortcut functions -----
template<typename T>
void ParseAs(const char * str, T & ret) {
    auto node = Parse(str);
    node >> ret;
}
template<typename T>
void ParseAs(std::string const& str, T & ret) {
    auto node = Parse(str);
    node >> ret;
}
template<typename T>
std::string ToString(T const& value) {
    return (Node() << value).ToString();
}

} // namespace json
} // namespace p2pd

#endif // P2PD_JSON_IO_H