#ifndef P2PD_API_CONTROLLER_H
#define P2PD_API_CONTROLLER_H

#include <functional>
#include <map>
#include <memory>
#include <string>

#include <boost/asio.hpp>

#include "api/service.h"
#include "engine/engine.h"
#include "engine/observer.h"

namespace p2pd {
namespace api {

using engine_ptr = std::shared_ptr<p2pd::engine::Engine>;

/**
 * @brief   Controler is the brain of API server. It receives requests from 
 *          server, performs them in engine, and send response to server. It
 *          also receives event from engine, and publish to server.
 * 
 * @author  deadblue
 */
class Controller : public engine::Observer {

private:
    using callback = std::function<void(std::string const&)>;
    using executor    = boost::asio::thread_pool;
    using service_ptr = std::unique_ptr<Service>;

    engine_ptr engine_;
    callback event_cb_;
    executor executor_{};
    std::map<std::string, service_ptr> services_{};

public:
    // Constructor
    Controller(engine_ptr engine, callback event_cb);
    // Destructor
    ~Controller() = default;

    // Execute request asynchronously.
    void AsyncExecute(std::string request, callback cb);

    // Override |engine::Observer|
    void OnEngineAlert(std::string const& message) final;
    void OnTaskStateChanged(uint32_t task_id, engine::TaskState state) final;

private:
    void DoExecute(std::string request, callback cb);

};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_CONTROLLER_H