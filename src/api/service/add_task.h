#ifndef P2PD_API_SERVICE_ADD_TASK_H
#define P2PD_API_SERVICE_ADD_TASK_H

#include "api/service.h"

namespace p2pd {
namespace api {
namespace service {

class AddTask: public Service {

public:
    AddTask(engine_ptr engine) : Service(engine) {}

    static const char * method() {
        return "add_task";
    }
    int Execute(
        json::Node const& params, json::Node & result
    ) final;

private:
    void AddMagnet(std::string & uri);
    void AddTorrent(std::string & data);

};

} // namespace service
} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVICE_ADD_TASK_H