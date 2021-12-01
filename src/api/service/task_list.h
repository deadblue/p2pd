#ifndef P2PD_API_SERVICE_TASK_LIST_H
#define P2PD_API_SERVICE_TASK_LIST_H

#include "api/service.h"

namespace p2pd {
namespace api {
namespace service {

class TaskList final : public Service {
public:
    // Override |p2pd:api::Service|
    const char * method() noexcept final;
    int Execute(
        json::Node const& params, json::Node & result
    ) final;
};

} // namespace service
} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVICE_TASK_LIST_H