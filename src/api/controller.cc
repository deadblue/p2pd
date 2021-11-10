#include "api/controller.h"

#include "api/protocol.h"
#include "api/service/add_task.h"
#include "json/io.h"

namespace p2pd {
namespace api {

Controller::Controller(
    engine_ptr engine, callback event_cb
) : engine_(engine), 
    event_cb_(std::move(event_cb)) {
    // Register self as observer to engine.
    engine_->AddObserver(this);
    // Register services.
    services_.emplace(
        service::AddTask::method(), 
        new service::AddTask(engine)
    );
}

// ----- Override |engine::Observer| -----

void Controller::OnEngineAlert(std::string const& message) {
    // TODO
}

void Controller::OnTaskStateChanged(uint32_t task_id, engine::TaskState state) {
    // TODO
}

// ----- Private methods -----

void Controller::AsyncExecute(std::string request, callback cb) {
    boost::asio::post(executor_, std::bind(
        &Controller::DoExecute, this, std::move(request), cb
    ));
}

void Controller::DoExecute(std::string request, callback cb) {
    // Parsing request
    auto req = Request();
    json::ParseAs(request, req);
    // Make response
    auto resp = Response();
    resp.id = req.id;
    if(services_.count(req.method) == 0) {
        resp.error = Error::UnsupportedMethod;
    } else {
        auto & service = services_[req.method];
        resp.error = service->Execute(req.params, resp.result);
    }
    cb(json::ToString(resp));
}

} // namespace api
} // namespace p2pd