#ifndef P2PD_API_SERVICE_ADD_TASK_H
#define P2PD_API_SERVICE_ADD_TASK_H

#include "api/service.h"

namespace p2pd {
namespace api {
namespace service {

struct AddTaskParams {
    std::string type;
    std::string uri;
};

struct AddTaskResult {
    std::string task_id;
};

class AddTask: public Service {

public:
    AddTask(engine_ptr engine) : Service(engine) {}

    static const char * method() {
        return "task.add";
    }
    int Execute(
        json::Node const& params, json::Node & result
    ) final;

private:
    using error_code = engine::error_code;

    void AddMagnet(std::string & uri, error_code & ec);
    void AddTorrent(std::string & data, error_code & ec);

};

} // namespace service
} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVICE_ADD_TASK_H