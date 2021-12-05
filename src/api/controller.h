#ifndef P2PD_API_CONTROLLER_H
#define P2PD_API_CONTROLLER_H

#include <atomic>
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
 *          server, performs them in engine, then send response to server. It
 *          also receives events from engine, and transfer them to server.
 * 
 * @author  deadblue
 */
class Controller final : public engine::Observer {

private:
    using engine_ptr  = std::shared_ptr<engine::Engine>;
    using service_ptr = std::unique_ptr<Service>;
    using callback = std::function<void(std::string)>;
    using executor = boost::asio::thread_pool;

    engine_ptr engine_;
    callback event_cb_;

    executor executor_{};
    std::atomic_bool stop_{false};

    std::map<std::string, service_ptr> services_{};

public:
    // Constructor
    Controller(engine_ptr const& engine, callback event_cb);
    // Destructor
    ~Controller();

    // Execute request asynchronously.
    void AsyncExecute(std::string request, callback cb);
    void Stop();

    // Override |engine::Observer|
    void OnTaskCreated(
        engine::TaskMetadata const& metadata
    ) const final;
    void OnTaskMetadataReceived(
        engine::TaskMetadata const& metadata
    ) const final;
    void OnTaskStateChanged(
        std::string const& task_id, 
        engine::TaskSummary::State old_state,
        engine::TaskSummary::State new_state
    ) const final;

private:
    void Register(Service * serv);
    void DoExecute(std::string request, callback cb);

    template<typename T>
    void DispatchEvent(const char * name, T const& data) const;

};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_CONTROLLER_H