#include "api/controller.h"

#include "api/error.h"
#include "api/message.h"
#include "api/service/add_task.h"
#include "engine/engine.h"
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

void Controller::AsyncExecute(std::string request, callback cb) {
    boost::asio::post(executor_, std::bind(
        &Controller::DoExecute, this, std::move(request), cb
    ));
}

// ----- Override |engine::Observer| -----

void Controller::OnEngineAlert(std::string const& message) {
    // auto data = event::EngineAlert();
    // data.message = message;
    // DispatchEvent("engine.alert", data);
}

void Controller::OnTaskStateChanged(uint32_t task_id, engine::TaskState state) {
    // auto data = event::TaskStateChanged();
    // data.task_id = task_id;
    // data.state = static_cast<int>(state);
    // DispatchEvent("task.state_changed", data);
}

// ----- Private methods -----

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
    auto response = json::ToString(resp);
    DLOG << "Send response: " << response;
    cb(std::move(response));
}

template<typename T>
void Controller::DispatchEvent(const char * name, T const& data) {
    auto event = Event(name);
    // TODO: Generate event ID
    event.data << data;
    auto event_str = json::ToString(event);
    DLOG << "Dispatch event: " << event_str;
    event_cb_( std::move(event_str) );
}

} // namespace api
} // namespace p2pd