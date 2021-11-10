#ifndef P2PD_API_SERVICE_H
#define P2PD_API_SERVICE_H

#include <memory>

#include "engine/engine.h"
#include "json/node.h"

namespace p2pd {
namespace api {

class Service {

protected:
    using engine_ptr  = std::shared_ptr<p2pd::engine::Engine>;
    engine_ptr engine_;

public:
    // Constructor
    Service(engine_ptr engine): engine_(engine) {}
    // Destructor
    virtual ~Service() = default;

    virtual int Execute(
        json::Node const& params, json::Node & result
    ) = 0;

};

} // namespace api
} // namespace p2pd

#endif // P2PD_API_SERVICE_H