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

Node& Node::operator=(const Node& other) {
    if(node_ != nullptr) { json_object_put(node_); }
    if(other.node_ != nullptr) {
        node_ = json_object_get(other.node_);
    } else {
        node_ = nullptr;
    }
    return *this;
}

int Node::Length() const {
    if(node_ == nullptr || !json_object_is_type(node_, json_type_array)) {
        return 0;
    }
    auto al = json_object_get_array(node_);
    return array_list_length(al);
}

void Node::AddChild(const char * key, Node && value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    json_object_object_add(node_, key, value.node_);
    // Take ownership.
    value.node_ = nullptr;
}

void Node::AddChild(const char * key, Node const& value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    json_object_object_add(node_, key, json_object_get(value.node_));
}

template<>
void Node::AddChild(const char * key, bool value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    auto * child = json_object_new_boolean(value);
    json_object_object_add(node_, key, child);
}

template<>
void Node::AddChild(const char * key, int value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    auto * child = json_object_new_int(value);
    json_object_object_add(node_, key, child);
}

template<>
void Node::AddChild(const char * key, int64_t value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    auto * child = json_object_new_int64(value);
    json_object_object_add(node_, key, child);
}

template<>
void Node::AddChild(const char * key, uint64_t value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    auto * child = json_object_new_uint64(value);
    json_object_object_add(node_, key, child);
}

template<>
void Node::AddChild(const char * key, double value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    auto * child = json_object_new_double(value);
    json_object_object_add(node_, key, child);
}

template<>
void Node::AddChild(const char * key, const char * value) {
    if(node_ != nullptr && !json_object_is_type(node_, json_type_object)) {
        return;
    }
    if(node_ == nullptr) {
        node_ = json_object_new_object();
    }
    auto * child = json_object_new_string(value);
    json_object_object_add(node_, key, child);
}

void Node::AddChild(const char * key, std::string const& value) {
    AddChild(key, value.c_str());
}

void Node::AddChild(const char * key, std::string && value) {
    AddChild(key, value.c_str());
}

std::string Node::ToString() {
    if(node_ == nullptr) {
        return std::string();
    }
    auto * str = json_object_to_json_string(node_);
    return std::string(str);
}

Node Parse(const char * str) {
    return Node( json_tokener_parse(str) );
}
Node Parse(std::string const& str) {
    return Parse(str.c_str());
}

} // namespace json
} // namespace p2pd