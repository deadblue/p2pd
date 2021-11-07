#ifndef P2PD_API_REQUEST_H
#define P2PD_API_REQUEST_H

#include <string>

#include "json/node.h"

namespace p2pd {
namespace api {

struct Request {
    std::string id;
    std::string method;
    json::Node params;
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_REQUEST_H