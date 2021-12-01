#ifndef P2PD_API_MESSAGE_H
#define P2PD_API_MESSAGE_H

#include <cstdlib>
#include <string>

#include "json/node.h"

namespace p2pd {
namespace api {

/**
 * The incoming message which is sent from client.
 */
struct Request {
    std::string id;
    std::string method;
    json::Node params;
};

/**
 * The outgoing message that server sends to client to reply request.
 */
struct Response {
    std::string id;
    int error = 0;
    json::Node result;

    Response() = default;
    Response(std::string const& id): id(id) {}
};

/**
 * The outgoing message that server sends to all clients.
 */
struct Event {
    std::string name;
    json::Node data;

    Event() = default;
    Event(const char * name) : name(name) {}
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_MESSAGE_H