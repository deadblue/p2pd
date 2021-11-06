#ifndef P2PD_WEBSOCKET_SESSION_HOST_H
#define P2PD_WEBSOCKET_SESSION_HOST_H

#include <string>

namespace p2pd {
namespace websocket {

class Session;

class SessionHost {
public:
    // |OnSessionMessage| will be invoked when session receive text message 
    // from client.
    virtual void OnSessionMessage(
        Session * session, std::string message) = 0;

    // |OnSessionClose| will be invoked when session receive close message
    // from client.
    virtual void OnSessionClose(Session * session) = 0;
};

} // namespace websocket
} // namespace p2pd

#endif // P2PD_WEBSOCKET_SESSION_HOST_H
