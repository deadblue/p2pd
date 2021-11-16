#include "json/node.h"

#include <json.h>

namespace p2pd {
namespace json {

#define JSON_C_VERSION_MAKER(MAJOR, MINOR, PATCH) \
    ((MAJOR << 16) | (MINOR << 8) | PATCH)

#if JSON_C_VERSION_NUM < JSON_C_VERSION_MAKER(0, 14, 0)
#define json_object_new_uint64 json_object_new_int64
#define json_object_get_uint64 json_object_get_int64
#endif

static const char * EMPTY_STRING = "";

Node::Node(json_object * node) : node_(node) {}

Node::Node(Node && other): node_(other.node_) {
    other.node_ = nullptr;
}

Node::~Node() {
    if(node_ != nullptr) { json_object_put(node_); }
}

// ----- Overload subscript access operators -----
Node Node::operator[](const char * key) const {
    if(node_ == nullptr || 
        !json_object_is_type(node_, json_type_object)) {
        return Node();
    }
    auto * item = json_object_object_get(node_, key);
    if(item == nullptr) {
        return Node();
    } else {
        return Node(json_object_get(item));
    }
}
Node Node::operator[](int index) const {
    if(node_ == nullptr || 
        !json_object_is_type(node_, json_type_array)) {
        return Node();
    }
    auto al = json_object_get_array(node_);
    if(al == nullptr || al->length <= index) {
        return Node();
    }
    auto item = reinterpret_cast<json_object *>(al->array[index]);
    return Node(json_object_get(item));
}

// ----- Overload casting operators -----
Node::operator bool() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_boolean)) {
        return false;
    }
    return json_object_get_boolean(node_);
}
Node::operator int() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_int)) {
        return 0;
    }
    return json_object_get_int(node_);
}
Node::operator int64_t() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_int)) {
        return 0;
    }
    return json_object_get_int64(node_);
}
Node::operator uint64_t() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_int)) {
        return 0;
    }
    return json_object_get_uint64(node_);
}
Node::operator double() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_double)) {
        return 0.0;
    }
    return json_object_get_double(node_);
}
Node::operator const char *() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_string)) {
        return EMPTY_STRING;
    }
    return json_object_get_string(node_);
}

// ----- Overload assigning operators -----
Node& Node::operator=(bool value) {
    // TODO?
    if(node_ != nullptr) { json_object_put(node_); }
    node_ = json_object_new_double(value);
    return *this;
}
Node& Node::operator=(int value) {
    if(node_ != nullptr) { json_object_put(node_); }
    node_ = json_object_new_int(value);
    return *this;
}
Node& Node::operator=(int64_t value) {
    if(node_ != nullptr) { json_object_put(node_); }
    node_ = json_object_new_int64(value);
    return *this;
}
Node& Node::operator=(uint64_t value) {
    if(node_ != nullptr) { json_object_put(node_); }
    node_ = json_object_new_uint64(value);
    return *this;
}
Node& Node::operator=(double value) {
    if(node_ != nullptr) { json_object_put(node_); }
    node_ = json_object_new_double(value);
    return *this;
}
Node& Node::operator=(const char * value) {
    if(node_ != nullptr) { json_object_put(node_); }
    if(value != nullptr) {
        node_ = json_object_new_string(value);
    } else {
        node_ = nullptr;
    }
    return *this;
}
Node& Node::operator=(std::string const& value) {
    return (*this) = value.c_str();
}
Node& Node::operator=(std::string && value) {
    return (*this) = value.c_str();
}
Node& Node::operator=(Node const& other) {
    if(node_ != nullptr) { json_object_put(node_); }
    if(other.node_ != nullptr) {
        node_ = json_object_get(other.node_);
    } else {
        node_ = nullptr;
    }
    return *this;
}
Node& Node::operator=(Node && other) {
    if(node_ != nullptr) { json_object_put(node_); }
    if(other.node_ != nullptr) {
        node_ = other.node_;
        other.node_ = nullptr;
    } else {
        node_ = nullptr;
    }
    return *this;
}

// ----- Array operating methods -----
int Node::Length() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_array)) {
        return 0;
    }
    auto al = json_object_get_array(node_);
    return array_list_length(al);
}
void Node::Append(Node const& element) {
    if(!EnsureArray()) { return; }
    json_object_array_add(node_, 
        // Increase ref-count
        json_object_get(element.node_)
    );
}
void Node::Append(Node && element) {
    if(!EnsureArray()) { return; }
    json_object_array_add(node_, element.node_);
    // Take-off ownership.
    element.node_ = nullptr;
}

// ----- Object operating methods -----
void Node::Attach(const char * key, Node const& value) {
    if(!EnsureObject()) { return; }
    json_object_object_add(node_, key, 
        // Increase ref-count
        json_object_get(value.node_)
    );
}
void Node::Attach(const char * key, Node && value) {
    if(!EnsureObject()) { return; }
    json_object_object_add(node_, key, value.node_);
    // Take-off ownership.
    value.node_ = nullptr;
}

std::string Node::ToString() {
    if(node_ == nullptr) {
        return std::string();
    }
    auto * str = json_object_to_json_string(node_);
    return std::string(str);
}

// ----- Private methods -----
bool Node::EnsureArray() {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_array)) {
        return false;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_array();
    }
    return true;
}
bool Node::EnsureObject() {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return false;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    return true;
}

// ----- Helper functions ----
Node Parse(const char * str) {
    return Node( json_tokener_parse(str) );
}
Node Parse(std::string const& str) {
    return Parse(str.c_str());
}

} // namespace json
} // namespace p2pd