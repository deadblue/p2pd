#ifndef P2PD_API_RESPONSE_H
#define P2PD_API_RESPONSE_H

#include <cstdint>
#include <string>

#include "json/node.h"

namespace p2pd {
namespace api {

struct Response {
    std::string id;
    int error = 0;
    json::Node result;
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_RESPONSE_H