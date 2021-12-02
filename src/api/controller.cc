#include "api/controller.h"

#include "api/error.h"
#include "api/event.h"
#include "api/message.h"
#include "api/service/engine_version.h"
#include "api/service/task_add.h"
#include "api/service/task_inspect.h"
#include "api/service/task_list.h"
#include "engine/engine.h"
#include "json/io.h"
#include "log/log.h"

namespace p2pd {
namespace api {

Controller::Controller(
    engine_ptr const& engine, callback event_cb
) : engine_(engine), event_cb_(std::move(event_cb)) {
    // Register self as observer to engine.
    engine_->AddObserver(this);
    // Register services.
    Register(new service::EngineVersion(), engine_);
    Register(new service::TaskAdd(), engine_);
    Register(new service::TaskList(), engine_);
    Register(new service::TaskInspect(), engine_);
}

Controller::~Controller() {
    DLOG << "Controller released";
}

void Controller::AsyncExecute(std::string request, callback cb) {
    if(stop_) { return; }
    boost::asio::post(executor_, std::bind(
        &Controller::DoExecute, this, std::move(request), cb
    ));
}

void Controller::Stop() {
    if(stop_.exchange(true)) { return; }
    // FIXME: A more graceful way is to remove all pending jobs and wait all 
    // running jobs, but |asio::thread_pool| doesn't support that :(
    // Currently, simply interrupt all threads.
    executor_.stop();
    // Unregister observer from engine.
    engine_->RemoveObserver(this);
}

// ----- Override |engine::Observer| -----

void Controller::OnTaskCreated(
    engine::TaskMetadata const& metadata
) const {
    static const char * event_name = "task.created";
    DispatchEvent(event_name, metadata);
}

void Controller::OnTaskMetadataReceived(
    engine::TaskMetadata const& metadata
) const {
    static const char * event_name = "task.metadataReceived";
    DispatchEvent(event_name, metadata);
}

void Controller::OnTaskStateChanged(
    std::string const& task_id, 
    engine::TaskSummary::State old_state,
    engine::TaskSummary::State new_state
) const {
    static const char * event_name = "task.stateUpdated";
    event::TaskStateUpdated event{
        task_id, 
        static_cast<int>(old_state), 
        static_cast<int>(new_state)
    };
    DispatchEvent(event_name, event);
}

// ----- Private methods -----

void Controller::Register(Service * serv, engine_ptr const& engine) {
    serv->engine_ = engine;
    services_.emplace(serv->method(), serv);
}

void Controller::DoExecute(std::string request, callback cb) {
    DLOG << "Receive request: " << request;
    // Parsing request
    Request req;
    json::ParseAs(request, req);
    // Make response
    Response resp;
    resp.id = req.id;
    if(services_.count(req.method) == 0) {
        resp.error = errors::unsupported_method;
    } else {
        auto & service = services_[req.method];
        resp.error = service->Execute(req.params, resp.result);
    }
    auto response = json::ToString(resp);
    DLOG << "Send response: " << response;
    cb( std::move(response) );
}

template<typename T>
void Controller::DispatchEvent(const char * name, T const& data) const {
    Event event{name};
    event.data << data;
    auto event_str = json::ToString(event);
    DLOG << "Dispatch event: " << event_str;
    event_cb_( std::move(event_str) );
}

} // namespace api
} // namespace p2pd