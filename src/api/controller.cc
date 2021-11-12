#include "api/controller.h"

#include "api/protocol.h"
#include "api/service/add_task.h"
#include "json/io.h"
#include "log/log.h"

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
    // Event data
    auto data = event::EngineAlert();
    data.message = message;
    // Make event
    auto event = Event("engine.alert");
    event.data << std::move(data);
    std::string event_string = json::ToString(event);
    DLOG << "Publish event: " << event_string;
    event_cb_( std::move(event_string) );
}

void Controller::OnTaskStateChanged(uint32_t task_id, engine::TaskState state) {
    auto data = event::TaskStateChanged();
    data.task_id = task_id;
    data.state = static_cast<int>(state);
    // Make event
    auto event = Event();
    event.name = "task.state_changed";
    event.data << std::move(data);
    event_cb_( json::ToString(event) );
}

// ----- Private methods -----

void Controller::AsyncExecute(std::string request, callback cb) {
    boost::asio::post(executor_, std::bind(
        &Controller::DoExecute, this, std::move(request), cb
    ));
}

void Controller::DoExecute(std::string request, callback cb) {
    DLOG << "Receive request: " << request;
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
    auto response = json::ToString(std::move(resp));
    DLOG << "Sending response: " << response;
    cb(std::move(response));
}

} // namespace api
} // namespace p2pd