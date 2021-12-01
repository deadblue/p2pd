#ifndef P2PD_API_CONTROLLER_H
#define P2PD_API_CONTROLLER_H

#include <functional>
#include <map>
#include <memory>
#include <string>

#include <boost/asio.hpp>

#include "api/service.h"
#include "engine/observer.h"

namespace p2pd {

namespace engine {
class Engine;
} // namespace engine

namespace api {

class Service;

/**
 * @brief   Controler is the brain of API server. It receives requests from 
 *          server, performs them in engine, and send response to server. It
 *          also receives event from engine, and publish to server.
 * 
 * @author  deadblue
 */
class Controller final : public engine::Observer {

private:
    using engine_ptr  = std::shared_ptr<engine::Engine>;
    using service_ptr = std::unique_ptr<Service>;
    using callback = std::function<void(std::string)>;
    using executor = boost::asio::thread_pool;

    callback event_cb_;
    executor executor_{};
    std::map<std::string, service_ptr> services_{};

public:
    // Constructor
    Controller(engine_ptr const& engine, callback event_cb);
    // Destructor
    ~Controller() = default;

    // Execute request asynchronously.
    void AsyncExecute(std::string request, callback cb);

    // Override |engine::Observer|
    void OnTaskCreated(engine::TaskMetadata const& metadata) final;
    void OnTaskMetadataReceived(engine::TaskMetadata const& metadata) final;
    void OnTaskStateChanged(
        std::string const& task_id, 
        engine::TaskSummary::State old_state,
        engine::TaskSummary::State new_state
    ) final;

private:
    void Register(Service * serv, engine_ptr const& engine);
    void DoExecute(std::string request, callback cb);

    template<typename T>
    void DispatchEvent(const char * name, T const& data);

};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_CONTROLLER_H