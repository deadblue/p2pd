#ifndef P2PD_JSON_IO_H
#define P2PD_JSON_IO_H

#include <string>
#include <vector>

#include "json/node.h"

namespace p2pd {
namespace json {

// ----- Struct unmarshallers -----
template<typename T>
void operator>>(Node const& from, T & to);
template<typename T>
T& operator>>(Node const& from, T && to);

// ----- Struct marshallers -----
template<typename T>
void operator<<(Node & to, T const& from);
template<typename T>
Node& operator<<(Node && to, T const& from);

// ----- Vector unmarshallers -----
template<typename T>
std::vector<T> & operator>>=(std::vector<T> & to, Node const& from) {
    for(size_t i = 0; i < from.Length(); ++i) {
        to.push_back(from[i] >> T());
    }
    return to;
}

// ----- Vector marshallers -----
template<typename T>
Node & operator<<=(Node & to, std::vector<T> const& from) {
    for(T const& item : from) {
        to.Append(Node() << item);
    }
    return to;
}
template<typename T>
Node & operator<<=(Node && to, std::vector<T> const& from) {
    for(T const& item : from) {
        to.Append(Node() << item);
    }
    return to;
}

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