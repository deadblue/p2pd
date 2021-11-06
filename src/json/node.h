#ifndef P2PD_JSON_NODE_H
#define P2PD_JSON_NODE_H

#include <cstdint>
#include <string>

struct json_object;

namespace p2pd {
namespace json {

class Node {
public:
    // Constructor
    explicit Node(json_object * node = nullptr);
    // Copy constructor
    Node(Node const& other) = default;
    // Move constructor
    Node(Node && other);
    // Destructor
    ~Node();

    // Overloading opeatror
    Node operator[](const char * key) const;
    Node operator[](int index) const;

    // Overloading casting operator
    operator bool() const;
    operator int() const;
    operator int64_t() const;
    operator uint64_t() const;
    operator double() const;
    operator const char *() const;

    // Overloading assign operator
    Node& operator=(bool value);
    Node& operator=(int value);
    Node& operator=(int64_t value);
    Node& operator=(uint64_t value);
    Node& operator=(double value);
    Node& operator=(const char * value);
    Node& operator=(const Node & other);

    // Return array list when node is an array, or 0.
    int Length() const;

    // Add a child when node is an object, or do nothing.
    template<typename T>
    void AddChild(const char * key, T value);
    void AddChild(const char * key, std::string const& value);
    // Add a node child
    void AddChild(const char * key, Node const& value);
    void AddChild(const char * key, Node && value);

    // Stringify
    std::string ToString();

private:
    json_object * node_;

    template<typename T>
    friend T& operator<<(T && to, Node && from);
};

Node Parse(const char * str);
Node Parse(std::string const& str);

} // namespace json
} // namespace p2pd

#endif // P2PD_JSON_NODE_H