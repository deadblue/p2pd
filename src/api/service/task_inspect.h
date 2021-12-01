#ifndef P2PD_SERVICE_TASK_INSTPEC_H
#define P2PD_SERVICE_TASK_INSTPEC_H

#include "api/service.h"

namespace p2pd {
namespace api {
namespace service {

class TaskInspect final : public Service {
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

#endif // P2PD_SERVICE_TASK_INSTPEC_H