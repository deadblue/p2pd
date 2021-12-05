#ifndef P2PD_API_SERVICE_TASK_GET_METADATA_H
#define P2PD_API_SERVICE_TASK_GET_METADATA_H

#include "api/service.h"

namespace p2pd {
namespace api {
namespace service {

class TaskGetMetadata final : public Service {
public:
    const char * method() noexcept final;
    int Execute(
        json::Node const& params, json::Node & result
    ) final;

};

} // namespace service
} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVICE_TASK_GET_METADATA_H