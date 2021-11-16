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
    // Move constructor
    Node(Node && other);
    // Delete copy constructor
    Node(Node const& other) = delete;
    // Destructor
    ~Node();

    // Overloading subscript access opeatrors
    Node operator[](const char * key) const;
    Node operator[](int index) const;

    // Overloading casting operators
    operator bool() const;
    operator int() const;
    operator int64_t() const;
    operator uint64_t() const;
    operator double() const;
    operator const char *() const;

    // Overloading assign operators
    Node& operator=(bool value);
    Node& operator=(int value);
    Node& operator=(int64_t value);
    Node& operator=(uint64_t value);
    Node& operator=(double value);
    Node& operator=(const char * value);
    Node& operator=(std::string const& value);
    Node& operator=(std::string && value);
    Node& operator=(Node const& other);
    Node& operator=(Node && other);

    // Return array list when node is an array, or 0.
    int Length() const;
    // Append an element into node, when node is an array.
    void Append(Node const& element);
    void Append(Node && element);

    // Attach a child on node, when node is an object.
    void Attach(const char * key, Node const& value);
    void Attach(const char * key, Node && value);

    // Stringify
    std::string ToString();

private:
    json_object * node_;

    bool EnsureArray();
    bool EnsureObject();
};

Node Parse(const char * str);
Node Parse(std::string const& str);

} // namespace json
} // namespace p2pd

#endif // P2PD_JSON_NODE_H