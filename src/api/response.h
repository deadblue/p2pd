#ifndef P2PD_API_RESPONSE_H
#define P2PD_API_RESPONSE_H

#include <string>

#include "json/node.h"

namespace p2pd {
namespace api {

struct GenericResponse {
    std::string id;
    std::string action;
    bool error = false;
    json::Node result;
};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_RESPONSE_H